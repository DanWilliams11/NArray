#pragma once

#include <stdexcept>
#include <thread>
#include <typeinfo>

#include "NArrayRecursiveDefinition.h"

using namespace NArray;

// default constructor
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive()
{
	m_data = nullptr;
	m_external = false;
	m_new_size = 0;
	m_size = 0;
	m_values_array = nullptr;
}

// constructor for contiguous and distributed internally allocated memory
// provide this constructor to kick-start the recursion
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(NArraySupport::StorageOption storage_option, const Shape& shape, const U& value, bool reindex)
{
	if (storage_option == NArraySupport::ContiguousInternal)
	{
		U* values_array = new U[shape.GetTotalSize()];
		CreateNArrayRecursiveContiguousInternal(shape, values_array, value, reindex);
	}
	else if (storage_option == NArraySupport::DistributedInternal)
		CreateNArrayRecursiveDistributedInternal(shape, value, reindex);
	else if (storage_option == NArraySupport::ContiguousExternal)
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousExternal' expects a pointer-to-value to initialise the array with\n"));
	else
		throw(std::invalid_argument("NArray: Invalid storage option\n"));
}

// constructor for contiguous externally allocated memory
// provide this constructor to kick-start the recursion
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(NArraySupport::StorageOption storage_option, const Shape& shape, U*& values_array, bool reindex)
{
	if (storage_option == NArraySupport::ContiguousExternal)
		CreateNArrayRecursiveContiguousExternal(shape, values_array, reindex);
	else if (storage_option == NArraySupport::ContiguousInternal || storage_option == NArraySupport::DistributedInternal)
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousInternal' and 'DistributedInternal' expect a value to initialise the array with\n"));
	else
		throw(std::invalid_argument("NArray: Invalid storage option\n"));
}

// recursive constructor for contiguous internally allocated memory
// this constructor is part of the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(const Shape& shape, Index* index, int& position, U*& values_array, const U& value)
{
	Initialise(shape, false);
	for (int i = 0; i < GetSize(); i++)
	{
		if (index)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, position, values_array, value);
	}
}

// recursive constructor for contiguous externally allocated memory
// this constructor is part of the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(const Shape& shape, Index* index, int& position, U*& values_array)
{
	Initialise(shape, true);
	for (int i = 0; i < GetSize(); i++)
	{
		if (index)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, position, values_array);
	}
}

// recursive constructor for distributed internally allocated memory
// this constructor is part of the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(const Shape& shape, Index* index, const U& value)
{
	Initialise(shape, false);
	for (int i = 0; i < GetSize(); i++)
	{
		if (index)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, value);
	}
}

// copy constructor for all types of memory allocation
// provide this copy constructor to kick-start the recursion
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(const NArrayRecursive& m)
{
	CreateNArrayRecursive(m);
}

// recursive copy constructor for all types of memory allocation
// provide this copy constructor to continue the recursion
// this copy constructor is part of the recursion and will not be called directly by the client-code, but cannot make private (needs to be accessed from equivalent function in class templated at the level above; recursively templated classes do not constitute a class hierarchy, so protected is equally inappropriate)
template <class T, class U> NArrayRecursive<T, U>::NArrayRecursive(const NArrayRecursive& m, int& position, U*& values_array)
{
	Initialise(m);
	CreateNArrayRecursive(m, position, values_array);
}

// destructor
template <class T, class U> NArrayRecursive<T, U>::~NArrayRecursive()
{
	DestroyNArrayRecursive();
}

// assignment operator
template <class T, class U> NArrayRecursive<T, U>& NArrayRecursive<T, U>::operator =(const NArrayRecursive& m)
{
	DestroyNArrayRecursive();
	CreateNArrayRecursive(m);
	return *this;
}

// member access operator
template <class T, class U> T& NArrayRecursive<T, U>::operator [](int j) const
{
	return const_cast<T&>(GetData(j));
}

template <class T, class U> template <class S, class V> bool NArrayRecursive<T, U>::ApplySingleThreadedOperator(const Mask& mask, SingleThreadedOperator<S, V>& op, const S& in, S& out, const ILogicalComparable<bool>* comparator)
{
	bool out_val = true;
	if (comparator)
		out_val = comparator->GetIdentityElement();
	for (int i = 0; i < GetSize(); i++)
	{
		bool val = m_data[i]->ApplySingleThreadedOperator(mask, op, in, out, comparator);
		if (comparator)
			out_val = comparator->ApplyComparator(out_val, val);
	}
	return out_val;
}

template <class T, class U> template <class S1, class V1, class S2, class V2> bool NArrayRecursive<T, U>::ApplySingleThreadedTransform(const Mask& mask, SingleThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out, const ILogicalComparable<bool>* comparator)
{
	bool out_val = true;
	if (comparator)
		out_val = comparator->GetIdentityElement();
	for (int i = 0; i < GetSize(); i++)
	{
		bool val = m_data[i]->ApplySingleThreadedTransform(mask, tr, in, out, comparator);
		if (comparator)
			out_val = comparator->ApplyComparator(out_val, val);
	}
	return out_val;
}

template <class T, class U> template <class S, class V> void NArrayRecursive<T, U>::ApplyMultiThreadedOperator(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out)
{
	if (!op.UsingMask() || (op.UsingMask() && op.GetMaskAtDim(GetOrder()) == Mask::not_use) || NArraySupport::MultiThreading::GetThreadCount() >= NArraySupport::c_max_num_threads)
	{
		for (int i = 0; i < GetSize(); i++)
		{
			m_data[i]->ApplyMultiThreadedOperator(mask, op, in, out);
		}
	}
	else
	{
		void (NArrayRecursive<T, U>::* wibble) (const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out) = &NArrayRecursive<T, U>::ApplyMultiThreadedOperatorGo<S, V>;
		NArraySupport::MultiThreading::AssignNewThread(new std::thread(wibble, this, std::ref(mask), std::ref(op), std::ref(in), std::ref(out)));
	}
}

template <class T, class U> template <class S, class V> void NArrayRecursive<T, U>::ApplyMultiThreadedOperatorGo(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out)
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->ApplyMultiThreadedOperator(mask, op, in, out);
	}
	NArraySupport::MultiThreading::DecrementThreadCount();
}

template <class T, class U> template <class S1, class V1, class S2, class V2> void NArrayRecursive<T, U>::ApplyMultiThreadedTransform(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out)
{
	if (!tr.UsingMask() || (tr.UsingMask() && tr.GetMaskAtDim(GetOrder()) == Mask::not_use) || NArraySupport::MultiThreading::GetThreadCount() >= NArraySupport::c_max_num_threads)
	{
		for (int i = 0; i < GetSize(); i++)
		{
			m_data[i]->ApplyMultiThreadedTransform(mask, tr, in, out);
		}
	}
	else
	{
		void (NArrayRecursive<T, U>::* wibble) (const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out) = &NArrayRecursive<T, U>::ApplyMultiThreadedTransformGo<S1, V1, S2, V2>;
		NArraySupport::MultiThreading::AssignNewThread(new std::thread(wibble, this, std::ref(mask), std::ref(tr), std::ref(in), std::ref(out)));
	}
}

template <class T, class U> template <class S1, class V1, class S2, class V2> void NArrayRecursive<T, U>::ApplyMultiThreadedTransformGo(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out)
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->ApplyMultiThreadedTransform(mask, tr, in, out);
	}
	NArraySupport::MultiThreading::DecrementThreadCount();
}

// constructor helper for contiguous internally allocated memory
template <class T, class U> void NArrayRecursive<T, U>::CreateNArrayRecursiveContiguousInternal(const Shape& shape, U*& values_array, const U& value, bool reindex)
{
	Index idx; // have to create this here and not in Initialise so that it remains on the stack during recursion
	Index* index = Initialise(shape, idx, values_array, false, reindex);
	int position = 0;
	for (int i = 0; i < GetSize(); i++)
	{
		if (reindex)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, position, values_array, value);
	}
}

// constructor helper for contiguous externally allocated memory
template <class T, class U> void NArrayRecursive<T, U>::CreateNArrayRecursiveContiguousExternal(const Shape& shape, U*& values_array, bool reindex)
{
	Index idx; // have to create this here and not in Initialise so that it remains on the stack during recursion
	Index* index = Initialise(shape, idx, values_array, true, reindex);
	int position = 0;
	for (int i = 0; i < GetSize(); i++)
	{
		if (reindex)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, position, values_array);
	}
}

// constructor helper for distributed internally allocated memory
template <class T, class U> void NArrayRecursive<T, U>::CreateNArrayRecursiveDistributedInternal(const Shape& shape, const U& value, bool reindex)
{
	Index idx; // have to create this here and not in Initialise so that it remains on the stack during recursion
	U* values_array = nullptr;
	Index* index = Initialise(shape, idx, values_array, false, reindex);
	for (int i = 0; i < GetSize(); i++)
	{
		if (reindex)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i] = new T(shape, index, value);
	}
}

// copy constructor helper for all types of allocated memory - kick-start the recursion
template <class T, class U> void NArrayRecursive<T, U>::CreateNArrayRecursive(const NArrayRecursive& m)
{
	Initialise(m);
	int position = 0;
	CreateNArrayRecursive(m, position, m_values_array);
	U* temp = new U[m.GetTotalSize()];
	m.Write(temp);
	Read(temp);
	delete[] temp;
}

// copy constructor helper for all types of allocated memory - continue the recursion
template <class T, class U> void NArrayRecursive<T, U>::CreateNArrayRecursive(const NArrayRecursive& m, int& position, U*& values_array)
{
	m_shape = m.m_shape;
	m_new_size = m.m_new_size;
	m_size = m.m_size;
	m_external = m.m_external;
	m_data = new T*[GetSize()];
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i] = new T(*m.m_data[i], position, values_array);
	}
}

// destructor helper for all types of allocated memory
template <class T, class U> void NArrayRecursive<T, U>::DestroyNArrayRecursive()
{
	if (m_values_array && !m_external) // contiguous internally allocated memory
	{
		delete[] m_values_array;
		m_values_array = nullptr;
	}
	for (int i = 0; i < GetSize(); i++)
	{
		if (m_data && m_data[i])
		{
			delete m_data[i];
			m_data[i] = nullptr;
		}
	}
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
}

// constructor helper for for all types of allocated memory during kick-start
template <class T, class U> Index* NArrayRecursive<T, U>::Initialise(const Shape& shape, Index& idx, U*& values_array, bool external, bool reindex)
{
	Initialise(shape, external);
	m_values_array = values_array;
	Index* index = nullptr;
	if (reindex)
	{
		idx.ReIndex(GetOrder());
		index = &idx;
	}
	return index;
}

// constructor helper for for all types of allocated memory during kick-start and recursion
template <class T, class U> void NArrayRecursive<T, U>::Initialise(const Shape& shape, bool external)
{
	m_shape = shape;
	if (shape.GetOrder() == 0)
	{
		m_new_size = 0;
		m_size = 0;
	}
	else
	{
		m_new_size = shape.GetShapeAtDim(GetOrder());
		m_size = shape.GetShapeAtDim(GetOrder());
	}
	m_values_array = nullptr;
	m_external = external;
	m_data = new T*[GetSize()];
}

// copy constructor helper for all types of allocated memory
template <class T, class U> void NArrayRecursive<T, U>::Initialise(const NArrayRecursive& m)
{
	if (m.m_values_array && !m.m_external) // contiguous internal
		m_values_array = new U[m.m_shape.GetTotalSize()];
	else if (m.m_values_array && m.m_external) // contiguous external
		m_values_array = m.m_values_array;
	else
		m_values_array = nullptr; // distributed internal
}

// need this function to kick-start and continue the recursion
template <class T, class U> const T& NArrayRecursive<T, U>::GetData(int j) const
{
	int i = j - 1;
	if (i >= 0 && i < GetSize())
		return *(m_data[i]);
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::GetData'\n"));
}

// be wary of using this function if you have re-sized the array - it returns the size of the dimension as was when the array was constructed
// at the point of construction, all entries within a dimension have the same size, as determined by the shape
// after re-sizing however, different entries within a dimension can have different sizes and the original shape no longer describes the array as a whole
// this function is safe if you have re-shaped the array
template <class T, class U> int NArrayRecursive<T, U>::GetDimSize(int dim) const
{
	if (dim == 0)
		return c_scalar_size;
	else
		return m_shape.GetShapeAtDim(dim);
}

template <class T, class U> int NArrayRecursive<T, U>::GetOrder() const
{
	static int order = -1;

	if (order == -1)
	{
		if (typeid(*this) == typeid(NArrayType<U>::M1))
			order = c_vector_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M2))
			order = c_matrix_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M3))
			order = c_array3D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M4))
			order = c_array4D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M5))
			order = c_array5D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M6))
			order = c_array6D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M7))
			order = c_array7D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M8))
			order = c_array8D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M9))
			order = c_array9D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M10))
			order = c_array10D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M11))
			order = c_array11D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M12))
			order = c_array12D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M13))
			order = c_array13D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M14))
			order = c_array14D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M15))
			order = c_array15D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M16))
			order = c_array16D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M17))
			order = c_array17D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M18))
			order = c_array18D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M19))
			order = c_array19D_order;
		else if (typeid(*this) == typeid(NArrayType<U>::M20))
			order = c_array20D_order;
		// TODO extend here for higher dimensions as follows:
		// else if (typeid(*this) == typeid(NArrayType<U>::M<n>))
		// order = c_array<n>D_order;
		else
			throw(std::invalid_argument("NArray: Unrecognised type - have you extended the paramaterised typedefs, but not extended 'NArrayRecursive::GetOrder'?\n"));
	}

	return order;
}

// need this function to kick-start and continue the recursion
// be wary of using this function on distributed arrays - never use pointer arithmetic on the returned pointer from a distributed array
template <class T, class U> U* NArrayRecursive<T, U>::GetPointerToValue(const Index& index) const
{
	int i = index.GetIndexAtDim(GetOrder());
	if (i >= 0 && i < GetSize())
		return m_data[i]->GetPointerToValue(index);
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::GetPointerToValue'\n"));
}

// be wary of using this function if you have re-sized the array - it returns the shape of the array as was when the array was constructed
// at the point of construction, all entries within a dimension have the same size, as determined by the shape
// after re-sizing however, different entries within a dimension can have different sizes and the original shape no longer describes the array as a whole
// this function is safe if you have re-shaped the array
// note that the shape of the array is only stored in the outer-most dimension, so do not call this function from inner dimensions
template <class T, class U> const Shape& NArrayRecursive<T, U>::GetShape() const
{
	return m_shape;
}

template <class T, class U> int NArrayRecursive<T, U>::GetSize() const
{
	return m_size;
}

// need this function to kick-start and continue the recursion
template <class T, class U> int NArrayRecursive<T, U>::GetTotalSize() const
{
	int total_size = 0;
	for (int i = 0; i < GetSize(); i++)
	{
		total_size += m_data[i]->GetTotalSize();
	}
	return total_size;
}

// need this function to kick-start and continue the recursion
template <class T, class U> const U& NArrayRecursive<T, U>::GetValue(const Index& index) const
{
	int i = index.GetIndexAtDim(GetOrder());
	if (i >= 0 && i < GetSize())
		return m_data[i]->GetValue(index);
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::GetValue'\n"));
}

// need this function to kick-start and continue the recursion
template <class T, class U> U NArrayRecursive<T, U>::GetWindowedValue(const Index& index) const
{
	int i = index.GetIndexAtDim(GetOrder());
	if (i >= 0 && i < GetSize())
		return m_data[i]->GetWindowedValue(index);
	else
	{
		U val;
		GenericType gen_val(val);
		return gen_val.GetAdditiveIdentityElement();
	}
}

// this function is not recursive, but Write is
template <class T, class U> void NArrayRecursive<T, U>::PrettyPrint(std::ostream& output_stream, bool verbosity, const char* msg, bool include_indexes) const
{
	if (!verbosity)
		return;

	if (output_stream.good())
	{
		if (msg)
			output_stream << msg << std::endl;
		Write(output_stream, include_indexes);
	}
	else
		throw std::ios_base::failure("NArray: Output stream failed in 'NArrayRecursive::PrettyPrint'\n");
}

// need this function to kick-start and continue the recursion
template <class T, class U> void NArrayRecursive<T, U>::PutData(int j, const T& value)
{
	int i = j - 1;
	if (i >= 0 && i < GetSize())
	{
		if (m_data[i])
		{
			delete m_data[i];
			m_data[i] = nullptr;
		}
		m_data[i] = new T(value);
	}
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::PutData'\n"));
}

// need this function to kick-start and continue the recursion
template <class T, class U> void NArrayRecursive<T, U>::PutPointerToValue(const Index& index, U* pointer_to_value)
{
	int i = index.GetIndexAtDim(GetOrder());
	if (i >= 0 && i < GetSize())
		m_data[i]->PutPointerToValue(index, pointer_to_value);
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::PutPointerToValue'\n"));
}

// for resizing all types of allocated memory (this calls a non-member function to kick-start a recursive ReSize from the top)
// when applied to a contiguous array, this function will create a hybrid array (mix of contiguous and distributed) if the array is stretched
// all new entries created during stretching will be added as distributed and will no longer be contiguous in memory
// do not use this function on contiguous arrays if it is essential that the entries remain contiguous in memory
// be wary of using pointer arithmetic on the pointer returned by GetPointerToValue if the original contiguous array has been stretched
// rather, use this function on distributed arrays and use the Write function to serialise it out to a new contiguous array if needed
// note that re-sizing an array means that its shape no longer describes the array as a whole (see notes on GetDimSize and GetShape)
template <class T, class U> template <class S> void NArrayRecursive<T, U>::PutSize(S& in_out, int size, const U& value, bool reindex)
{
	m_new_size = size;
	::ReSize(in_out, value, reindex);
}

// need this function to kick-start and continue the recursion
template <class T, class U> void NArrayRecursive<T, U>::PutValue(const Index& index, const U& value)
{
	int i = index.GetIndexAtDim(GetOrder());
	if (i >= 0 && i < GetSize())
		m_data[i]->PutValue(index, value);
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'NArrayRecursive::PutValue'\n"));
}

// need this function to continue the recursion
template <class T, class U> template <class S> void NArrayRecursive<T, U>::Read(const S* values_array, int& position, const Mask* mask)
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->Read(values_array, position, mask);
	}
}

// need this function to kick-start the recursion
template <class T, class U> template <class S> void NArrayRecursive<T, U>::Read(const S* values_array, const Mask* mask)
{
	int position = 0;
	Read(values_array, position, mask);
}

template <class T, class U> void NArrayRecursive<T, U>::Read(std::istream& input_stream)
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->Read(input_stream);
	}
}

// for reshaping contiguous and distributed internally allocated memory
template <class T, class U> void NArrayRecursive<T, U>::ReShape(NArraySupport::StorageOption storage_option, const Shape& shape, const U& value, bool reindex)
{
	DestroyNArrayRecursive();
	if (storage_option == NArraySupport::ContiguousInternal)
	{
		U* values_array = new U[shape.GetTotalSize()];
		CreateNArrayRecursiveContiguousInternal(shape, values_array, value, reindex);
	}
	else if (storage_option == NArraySupport::DistributedInternal)
		CreateNArrayRecursiveDistributedInternal(shape, value, reindex);
	else
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousExternal' expects a pointer-to-value to initialise the n-dimensional array with\n"));
}

// for reshaping contiguous externally allocated memory
template <class T, class U> void NArrayRecursive<T, U>::ReShape(NArraySupport::StorageOption storage_option, const Shape& shape, U*& values_array, bool reindex)
{
	DestroyNArrayRecursive();
	if (storage_option == NArraySupport::ContiguousExternal)
		CreateNArrayRecursiveContiguousExternal(shape, values_array, reindex);
	else
		throw(std::invalid_argument("NArray: The storage option does not match the argument - 'ContiguousInternal' and 'DistributedInternal' expect a value to initialise the n-dimensional array with\n"));
}

// for resizing all types of allocated memory (this is a non-member function called by PutSize which kick-starts a recursive ReSize from the top)
template <class S, class V> void ReSize(S& in_out, const V& value, bool reindex)
{
	Index idx;
	Index* index = nullptr;
	if (reindex)
	{
		idx.ReIndex(in_out.GetOrder());
		index = &idx;
	}
	in_out.ReSize(index, value);
}

// for resizing all types of allocated memory - continue the recursion (a non-member function called by PutSize kick-starts the recursion from the top)
template <class T, class U> void NArrayRecursive<T, U>::ReSize(Index* index, const U& value)
{
	if (m_size != m_new_size)
	{
		T** temp_data = new T*[m_new_size];
		if (m_size < m_new_size)
		{
			for (int i = 0; i < m_size; i++)
				temp_data[i] = m_data[i];
			for (int i = m_size; i < m_new_size; i++)
			{
				if (index)
					index->PutIndexAtDim(GetOrder(), i);
				temp_data[i] = new T(m_shape, index, value);
			}
		}
		if (m_size > m_new_size)
		{
			for (int i = 0; i < m_new_size; i++)
				temp_data[i] = m_data[i];
			for (int i = m_new_size; i < m_size; i++)
				delete m_data[i];
		}
		delete m_data;
		m_data = temp_data;
		m_size = m_new_size;
	}
	for (int i = 0; i < GetSize(); i++)
	{
		if (index)
			index->PutIndexAtDim(GetOrder(), i);
		m_data[i]->ReSize(index, value);
	}
}

template <class T, class U> template <class S> void NArrayRecursive<T, U>::SerialiseNonUniform(S& in_out, std::fstream& ka, bool in, bool reindex)
{
	if (in)
	{
		int new_size = 0;
		ka >> new_size;
		if (reindex)
		{
			U value;
			PutSize(in_out, new_size, value, reindex);
		}
		else
		{
			m_new_size = new_size;
			U value;
			ReSize(nullptr, value);
		}
	}
	else
	{
		ka << GetSize() << ' ';
	}
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->SerialiseNonUniform(in_out, ka, in, reindex);
	}
}

template <class T, class U> void NArrayRecursive<T, U>::SerialiseUniform(NArraySupport::StorageOption storage_option, std::fstream& ka, bool in, bool reindex)
{
	if (in)
	{
		Shape shape(GetOrder());
		ka >> shape;
		U v;
		ReShape(storage_option, shape, v, reindex);
		Read(ka);
	}
	else
	{
		ka << GetShape();
		Write(ka);
	}
}

template <class T, class U> void NArrayRecursive<T, U>::SwapValue(const Index& index1, const Index& index2)
{
	U temp = GetValue(index1);
	PutValue(index1, GetValue(index2));
	PutValue(index2, temp);
}

// need this function to continue the recursion
template <class T, class U> template <class S> void NArrayRecursive<T, U>::Write(S* values_array, int& position, const Mask* mask) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->Write(values_array, position, mask);
	}
}

// need this function to kick-start the recursion
template <class T, class U> template <class S> void NArrayRecursive<T, U>::Write(S* values_array, const Mask* mask) const
{
	int position = 0;
	Write(values_array, position, mask);
}

template <class T, class U> void NArrayRecursive<T, U>::Write(std::ostream& output_stream, bool include_indexes) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		m_data[i]->Write(output_stream, include_indexes);
	}
	if (output_stream.good())
		output_stream << std::endl;
	else
		throw std::ios_base::failure("NArray: Output stream failed in 'NArrayRecursive::Write'\n");
}
