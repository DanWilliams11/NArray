#pragma once

#include <stdexcept>
#include <thread>
#include <typeinfo>

#include "NArrayAxiomaticDefinition.h"

using namespace NArray;

// default constructor
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic()
{
	m_distributed = true;
	m_value = nullptr;
}

// constructor for contiguous and distributed internally allocated memory
// provide this constructor if not recursing i.e. creating a scalar
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(NArraySupport::StorageOption storage_option, const U& value)
{
	if (storage_option == NArraySupport::ContiguousInternal || storage_option == NArraySupport::DistributedInternal)
	{
		m_distributed = true; // internally allocated scalars do not have a values array, they only have a single value, so contiguity does not apply - mark as distributed in both cases to ensure destruction
		m_value = new U(value);
	}
	else if (storage_option == NArraySupport::ContiguousExternal)
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousExternal' expects a pointer-to-value to initialise the scalar with\n"));
	else
		throw(std::invalid_argument("NArray: Invalid storage option\n"));
}

// constructor for contiguous externally allocated memory
// provide this constructor if not recursing i.e. creating a scalar
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(NArraySupport::StorageOption storage_option, U*& value)
{
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		m_distributed = false; // externally allocated scalars do not have a values array, they only have a single value, so contiguity does not apply, but the value belongs to memory allocated outside the scalar's control - mark as not-distributed to ensure NO destruction
		m_value = value;
	}
	else if (storage_option == NArraySupport::ContiguousInternal || storage_option == NArraySupport::DistributedInternal)
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousInternal' and 'DistributedInternal' expect a value to initialise the scalar with\n"));
	else
		throw(std::invalid_argument("NArray: Invalid storage option\n"));
}

// axiomatic constructor for contiguous internally allocated memory
// this constructor bottoms-out the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
// at the bottom of the recursion shape is not used and index will not change
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(const Shape& shape, const Index* index, int& position, U*& values_array, const U& value)
{
	shape; // do this to prevent compiler warning 'unreferenced formal parameter'
	CreateNArrayAxiomaticContiguousInternal(index, position, values_array, value);
}

// axiomatic constructor for contiguous externally allocated memory
// this constructor bottoms-out the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
// at the bottom of the recursion shape is not used and index will not change
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(const Shape& shape, const Index* index, int& position, U*& values_array)
{
	shape; // do this to prevent compiler warning 'unreferenced formal parameter'
	CreateNArrayAxiomaticContiguousExternal(index, position, values_array);
}

// axiomatic constructor for distributed internally allocated memory
// this constructor bottoms-out the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
// at the bottom of the recursion shape is not used and index will not change
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(const Shape& shape, const Index* index, const U& value)
{
	shape; // do this to prevent compiler warning 'unreferenced formal parameter'
	CreateNArrayAxiomaticDistributedInternal(index, value);
}

// copy constructor for all types of memory allocation
// provide this copy constructor if not recursing i.e. copying a scalar
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(const NArrayAxiomatic& m)
{
	CreateNArrayAxiomatic(m);
}

// axiomatic copy constructor for all types of memory allocation
// provide this copy constructor to bottom-out the recursion
// this copy constructor is part of the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
template <class U> NArrayAxiomatic<U>::NArrayAxiomatic(const NArrayAxiomatic& m, int& position, U*& values_array)
{
	CreateNArrayAxiomatic(m, position, values_array);
}

// destructor
template <class U> NArrayAxiomatic<U>::~NArrayAxiomatic()
{
	DestroyNArrayAxiomatic();
}

// assignment operator
template <class U> NArrayAxiomatic<U>& NArrayAxiomatic<U>::operator =(const NArrayAxiomatic& m)
{
	DestroyNArrayAxiomatic();
	CreateNArrayAxiomatic(m);
	return *this;
}

// member access operator
template <class U> U& NArrayAxiomatic<U>::operator [](int j) const
{
	return const_cast<U&>(GetData(j));
}

template <class U> template <class S, class V> bool NArrayAxiomatic<U>::ApplySingleThreadedOperator(const Mask& mask, SingleThreadedOperator<S, V>& op, const S& in, S& out, const ILogicalComparable<bool>* comparator)
{
	bool out_val = true;
	if (comparator)
		out_val = comparator->GetIdentityElement();
	if (NArraySupport::CompareIndexToMask(m_index, mask))
		out_val = op.ApplyOperator(m_index, *m_value, in, out);
	return out_val;
}

template <class U> template <class S1, class V1, class S2, class V2> bool NArrayAxiomatic<U>::ApplySingleThreadedTransform(const Mask& mask, SingleThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out, const ILogicalComparable<bool>* comparator)
{
	bool out_val = true;
	if (comparator)
		out_val = comparator->GetIdentityElement();
	if (NArraySupport::CompareIndexToMask(m_index, mask))
		out_val = tr.ApplyTransform(m_index, *m_value, in, out);
	return out_val;
}

template <class U> template <class S, class V> void NArrayAxiomatic<U>::ApplyMultiThreadedOperator(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out)
{
	if (NArraySupport::CompareIndexToMask(m_index, mask))
	{
		if (!op.UsingMask() || (op.UsingMask() && op.GetMaskAtDim(GetOrder()) == Mask::not_use) || NArraySupport::MultiThreading::GetThreadCount() >= NArraySupport::c_max_num_threads)
		{
			op.ApplyOperator(m_index, *m_value, in, out);
		}
		else
		{
			void (NArrayAxiomatic<U>::* wibble) (MultiThreadedOperator<S, V>& op, const S& in, S& out) = &NArrayAxiomatic<U>::ApplyMultiThreadedOperatorGo<S, V>;
			NArraySupport::MultiThreading::AssignNewThread(new std::thread(wibble, this, std::ref(op), std::ref(in), std::ref(out)));
		}
	}
}

template <class U> template <class S, class V> void NArrayAxiomatic<U>::ApplyMultiThreadedOperatorGo(MultiThreadedOperator<S, V>& op, const S& in, S& out)
{
	op.ApplyOperator(m_index, *m_value, in, out);
	NArraySupport::MultiThreading::DecrementThreadCount();
}

template <class U> template <class S1, class V1, class S2, class V2> void NArrayAxiomatic<U>::ApplyMultiThreadedTransform(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out)
{
	if (NArraySupport::CompareIndexToMask(m_index, mask))
	{
		if (!tr.UsingMask() || (tr.UsingMask() && tr.GetMaskAtDim(GetOrder()) == Mask::not_use) || NArraySupport::MultiThreading::GetThreadCount() >= NArraySupport::c_max_num_threads)
		{
			tr.ApplyTransform(m_index, *m_value, in, out);
		}
		else
		{
			void (NArrayAxiomatic<U>::* wibble) (MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out) = &NArrayAxiomatic<U>::ApplyMultiThreadedTransformGo<S1, V1, S2, V2>;
			NArraySupport::MultiThreading::AssignNewThread(new std::thread(wibble, this, std::ref(tr), std::ref(in), std::ref(out)));
		}
	}
}

template <class U> template <class S1, class V1, class S2, class V2> void NArrayAxiomatic<U>::ApplyMultiThreadedTransformGo(MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out)
{
	tr.ApplyTransform(m_index, *m_value, in, out);
	NArraySupport::MultiThreading::DecrementThreadCount();
}

// constructor helper for contiguous internally allocated memory
template <class U> void NArrayAxiomatic<U>::CreateNArrayAxiomaticContiguousInternal(const Index* index, int& position, U*& values_array, const U& value)
{
	m_distributed = false;
	if (index)
		m_index = *index;
	m_value = &values_array[position];
	*m_value = value;
	position++;
}

// constructor helper for contiguous externally allocated memory
template <class U> void NArrayAxiomatic<U>::CreateNArrayAxiomaticContiguousExternal(const Index* index, int& position, U*& values_array)
{
	m_distributed = false;
	if (index)
		m_index = *index;
	m_value = &values_array[position];
	position++;
}

// constructor helper for distributed internally allocated memory
template <class U> void NArrayAxiomatic<U>::CreateNArrayAxiomaticDistributedInternal(const Index* index, const U& value)
{
	m_distributed = true;
	if (index)
		m_index = *index;
	m_value = new U(value);
}

// copy constructor helper for all types of allocated memory - if not recursing i.e. copying a scalar
template <class U> void NArrayAxiomatic<U>::CreateNArrayAxiomatic(const NArrayAxiomatic& m)
{
	m_distributed = m.m_distributed;
	m_index = m.m_index;
	if (m.m_distributed)
	{
		m_value = new U(*m.m_value);
	}
	else
	{
		m_value = m.m_value;
	}
}

// copy constructor helper for all types of allocated memory - bottom-out the recursion
template <class U> void NArrayAxiomatic<U>::CreateNArrayAxiomatic(const NArrayAxiomatic& m, int& position, U*& values_array)
{
	m_distributed = m.m_distributed;
	m_index = m.m_index;
	if (m.m_distributed)
	{
		m_value = new U(*m.m_value);
	}
	else
	{
		m_value = &values_array[position];
		position++;
	}
}

// destructor helper for all types of allocated memory
template <class U> void NArrayAxiomatic<U>::DestroyNArrayAxiomatic()
{
	if (m_value && m_distributed) // distributed internally allocated memory
	{
		delete m_value;
		m_value = nullptr;
	}
}

// need this function to bottom-out the recursion
template <class U> const U& NArrayAxiomatic<U>::GetData(int j) const
{
	j; // j is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	return GetData();
}

// need this function if not recursing i.e. calling from a scalar
template <class U> const U& NArrayAxiomatic<U>::GetData() const
{
	return *m_value;
}

template <class U> const Index& NArrayAxiomatic<U>::GetIndex() const
{
	return m_index;
}

template <class U> int NArrayAxiomatic<U>::GetOrder() const
{
	return c_scalar_order;
}

// need this function to bottom-out the recursion
// be wary of using this function on distributed arrays - never use pointer arithmetic on the returned pointer from a distributed array
template <class U> U* NArrayAxiomatic<U>::GetPointerToValue(const Index& index) const
{
	index; // index is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	return GetPointerToValue();
}

// need this function if not recursing i.e. calling from a scalar
// be wary of using this function on distributed arrays - never use pointer arithmetic on the returned pointer from a distributed array
template <class U> U* NArrayAxiomatic<U>::GetPointerToValue() const
{
	return m_value;
}

// need this function to satisfy the compiler
template <class U> const Shape& NArrayAxiomatic<U>::GetShape() const
{
	static const Shape shape; // scalars do not have a shape, they only have a single value - make a shape up to satisfy the compiler
	return shape;
}

template <class U> int NArrayAxiomatic<U>::GetSize() const
{
	return c_scalar_size;
}

// need this function to bottom-out the recursion and if not recursing i.e. calling from a scalar
template <class U> int NArrayAxiomatic<U>::GetTotalSize() const
{
	return GetSize();
}

// need this function to bottom-out the recursion
template <class U> const U& NArrayAxiomatic<U>::GetValue(const Index& index) const
{
	index; // index is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	return GetValue();
}

// need this function if not recursing i.e. calling from a scalar
template <class U> const U& NArrayAxiomatic<U>::GetValue() const
{
	return *m_value;
}

// need this function to bottom-out the recursion
template <class U> U NArrayAxiomatic<U>::GetWindowedValue(const Index& index) const
{
	index; // index is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	return GetWindowedValue();
}

// need this function if not recursing i.e. calling from a scalar
template <class U> U NArrayAxiomatic<U>::GetWindowedValue() const
{
	return *m_value;
}

// this function is not recursive, but Write is
template <class U> void NArrayAxiomatic<U>::PrettyPrint(std::ostream& output_stream, bool verbosity, const char* msg, bool include_indexes) const
{
	if (!verbosity)
		return;

	if (output_stream.good())
	{
		if (msg)
			output_stream << msg << std::endl;
		Write(output_stream, include_indexes);
		output_stream << std::endl;
	}
	else
		throw std::ios_base::failure("NArray: Output stream failed in 'NArrayAxiomatic::PrettyPrint'\n");
}

// need this function to bottom-out the recursion
template <class U> void NArrayAxiomatic<U>::PutData(int j, const U& value)
{
	j; // j is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	PutData(value);
}

// need this function if not recursing i.e. calling from a scalar
template <class U> void NArrayAxiomatic<U>::PutData(const U& value)
{
	*m_value = value;
}

// need this function to bottom-out the recursion
template <class U> void NArrayAxiomatic<U>::PutPointerToValue(const Index& index, U* pointer_to_value)
{
	index; // index is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	PutPointerToValue(pointer_to_value);
}

// need this function if not recursing i.e. calling from a scalar
template <class U> void NArrayAxiomatic<U>::PutPointerToValue(U* pointer_to_value)
{
	m_value = pointer_to_value;
}

// need this function to bottom-out the recursion
template <class U> void NArrayAxiomatic<U>::PutValue(const Index& index, const U& value)
{
	index; // index is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	PutValue(value);
}

// need this function if not recursing i.e. calling from a scalar
template <class U> void NArrayAxiomatic<U>::PutValue(const U& value)
{
	*m_value = value;
}

// need this function to bottom-out the recursion
template <class U> template <class S> void NArrayAxiomatic<U>::Read(const S* values_array, int& position, const Mask* mask)
{
	bool cont = false;
	if (!mask)
		cont = true;
	else if (NArraySupport::CompareIndexToMask(m_index, *mask))
		cont = true;

	if (cont)
	{
		if (values_array)
		{
			*m_value = values_array[position];
			position++;
		}
		else
		{
			throw std::ios_base::failure("NArray: Input array failed in 'NArrayAxiomatic::Read'\n");
		}
	}
}

// need this function if not recursing i.e. calling from a scalar
template <class U> template <class S> void NArrayAxiomatic<U>::Read(const S* values_array)
{
	int position = 0;
	Read(values_array, position);
}

template <class U> void NArrayAxiomatic<U>::Read(std::istream& input_stream)
{
	if (input_stream.good())
		input_stream >> *m_value;
	else
		throw std::ios_base::failure("NArray: Input stream failed in 'NArrayAxiomatic::Read'\n");
}

// for reshaping contiguous and distributed internally allocated memory
template <class U> void NArrayAxiomatic<U>::ReShape(NArraySupport::StorageOption storage_option, const U& value)
{
	DestroyNArrayAxiomatic();
	if (storage_option == NArraySupport::ContiguousInternal || storage_option == NArraySupport::DistributedInternal)
	{
		m_distributed = true; // scalars do not have a values array, they only have a single value, so are inherently contiguous - mark as distributed to ensure destruction
		m_value = new U(value);
	}
	else
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousExternal' expects a pointer-to-value to initialise the scalar with\n"));
}

// for reshaping contiguous externally allocated memory
template <class U> void NArrayAxiomatic<U>::ReShape(NArraySupport::StorageOption storage_option, U*& value)
{
	DestroyNArrayAxiomatic();
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		m_distributed = false;
		m_value = value;
	}
	else
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousInternal' and 'DistributedInternal' expect a value to initialise the scalar with\n"));
}

// for resizing all types of allocated memory - bottom-out the recursion
template <class U> void NArrayAxiomatic<U>::ReSize(Index* index, const U& value)
{
	value; // value is not used in the axiomatic case (value is used in the final recursive case (vector) and you cannot resize a scalar) - do this to prevent compiler warning 'unreferenced formal parameter'
	if (index)
		m_index = *index;
}

template <class U> template <class S> void NArrayAxiomatic<U>::SerialiseNonUniform(S& in_out, std::fstream& ka, bool in, bool reindex)
{
	in_out; // in_out is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	reindex; // reindex is not used in the axiomatic case - do this to prevent compiler warning 'unreferenced formal parameter'
	if (in)
		ka >> *m_value;
	else
		ka << *m_value << ' ';
}

// need this function to bottom-out the recursion
template <class U> template <class S> void NArrayAxiomatic<U>::Write(S* values_array, int& position, const Mask* mask) const
{
	bool cont = false;
	if (!mask)
		cont = true;
	else if (NArraySupport::CompareIndexToMask(m_index, *mask))
		cont = true;

	if (cont)
	{
		if (values_array)
		{
			values_array[position] = *m_value;
			position++;
		}
		else
		{
			throw std::ios_base::failure("NArray: Output array failed in 'NArrayAxiomatic::Write'\n");
		}
	}
}

// need this function if not recursing i.e. calling from a scalar
template <class U> template <class S> void NArrayAxiomatic<U>::Write(S* values_array) const
{
	int position = 0;
	Write(values_array, position);
}

template <class U> void NArrayAxiomatic<U>::Write(std::ostream& output_stream, bool include_indexes) const
{
	if (output_stream.good())
	{
		if (include_indexes)
			output_stream << m_index << ": ";
		output_stream << *m_value << ' ';
	}
	else
		throw std::ios_base::failure("NArray: Output stream failed in 'NArrayAxiomatic::Write'\n");
}
