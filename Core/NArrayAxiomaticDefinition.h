// the functions in this file are for bottoming-out the recursion initiated by NArrayRecursive and if not recursing i.e. calling from a scalar
// note that the functions called during recursion are not the same function, they are functions with the same signature belonging to classes templated at the next level down, eventually culminating in the functions in this file
// apart from a few axiomatic-specific functions, the functions in this file mirror those in NArrayRecursive

#pragma once

#include <iostream>

#include "Index.h"
#include "Mask.h"
#include "Operator.h"
#include "Shape.h"
#include "NArraySupport.h"
#include "Transform.h"

namespace NArray
{
	template <class U> class NArrayAxiomatic
	{
		// member variables
	private:
				bool m_distributed;
				Index m_index;
				U* m_value;
		// member functions
	private:
				void CreateNArrayAxiomaticContiguousInternal(const Index* index, int& position, U*& values_array, const U& value); // constructor helper for contiguous internally allocated memory
				void CreateNArrayAxiomaticContiguousExternal(const Index* index, int& position, U*& values_array); // constructor helper for contiguous externally allocated memory
				void CreateNArrayAxiomaticDistributedInternal(const Index* index, const U& value); // constructor helper for distributed internally allocated memory
				void CreateNArrayAxiomatic(const NArrayAxiomatic& m); // copy constructor helper (if copying a scalar) for all types of allocated memory
				void CreateNArrayAxiomatic(const NArrayAxiomatic& m, int& position, U*& values_array); // copy constructor helper (if recursing) for all types of allocated memory
				void DestroyNArrayAxiomatic(); // destructor helper for all types of allocated memory
	public:
				NArrayAxiomatic(); // default constructor
				NArrayAxiomatic(NArraySupport::StorageOption storage_option, const U& value); // constructor (if creating a scalar) for contiguous and distributed internally allocated memory
				NArrayAxiomatic(NArraySupport::StorageOption storage_option, U*& value); // constructor (if creating a scalar) for contiguous externally allocated memory
				NArrayAxiomatic(const Shape& shape, const Index* index, int& position, U*& values_array, const U& value); // axiomatic constructor (bottoms-out the recursion) for contiguous internally allocated memory
				NArrayAxiomatic(const Shape& shape, const Index* index, int& position, U*& values_array); // axiomatic constructor (bottoms-out the recursion) for contiguous externally allocated memory
				NArrayAxiomatic(const Shape& shape, const Index* index, const U& value); // axiomatic constructor (bottoms-out the recursion) for distributed internally allocated memory
				NArrayAxiomatic(const NArrayAxiomatic& m); // copy constructor (if copying a scalar) for all types of memory allocation
				NArrayAxiomatic(const NArrayAxiomatic& m, int& position, U*& values_array); // axiomatic copy constructor (bottoms-out the recursion) for all types of memory allocation
		virtual ~NArrayAxiomatic(); // destructor
				NArrayAxiomatic& operator =(const NArrayAxiomatic& m); // assignment operator
				U& operator [](int j) const; // member access operator
				template <class S, class V> bool ApplySingleThreadedOperator(const Mask& mask, SingleThreadedOperator<S, V>& op, const S& in, S& out, const ILogicalComparable<bool>* comparator = nullptr);
				template <class S1, class V1, class S2, class V2> bool ApplySingleThreadedTransform(const Mask& mask, SingleThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out, const ILogicalComparable<bool>* comparator = nullptr);
				template <class S, class V> void ApplyMultiThreadedOperator(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out);
				template <class S, class V> void ApplyMultiThreadedOperatorGo(MultiThreadedOperator<S, V>& op, const S& in, S& out);
				template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransform(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out);
				template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransformGo(MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out);
				const U& GetData(int j) const; // need this function to bottom-out the recursion
				const U& GetData() const; // need this function if not recursing i.e. calling from a scalar
				const Index& GetIndex() const;
				int GetOrder() const;
				U* GetPointerToValue(const Index& index) const; // need this function to bottom-out the recursion - see warning in cpp file
				U* GetPointerToValue() const; // need this function if not recursing i.e. calling from a scalar - see warning in cpp file
				const Shape& GetShape() const; // need this function to satisfy the compiler - see warning in cpp file
				int GetSize() const;
				int GetTotalSize() const; // need this function to bottom-out the recursion and if not recursing i.e. calling from a scalar
				const U& GetValue(const Index& index) const; // need this function to bottom-out the recursion
				const U& GetValue() const; // need this function if not recursing i.e. calling from a scalar
				U GetWindowedValue(const Index& index) const;  // need this function to bottom-out the recursion
				U GetWindowedValue() const; // need this function if not recursing i.e. calling from a scalar
				void PrettyPrint(std::ostream& output_stream, bool verbosity, const char* msg = nullptr, bool include_indexes = false) const;
				void PutData(int j, const U& value); // need this function to bottom-out the recursion
				void PutData(const U& value); // need this function if not recursing i.e. calling from a scalar
				void PutPointerToValue(const Index& index, U* pointer_to_value); // need this function to bottom-out the recursion
				void PutPointerToValue(U* pointer_to_value); // need this function if not recursing i.e. calling from a scalar
				void PutValue(const Index& index, const U& value); // need this function to bottom-out the recursion
				void PutValue(const U& value); // need this function if not recursing i.e. calling from a scalar
				template <class S> void Read(const S* values_array, int& position, const Mask* mask = nullptr); // need this function to bottom-out the recursion
				template <class S> void Read(const S* values_array); // need this function if not recursing i.e. calling from a scalar
				void Read(std::istream& input_stream);
				void ReShape(NArraySupport::StorageOption storage_option, const U& value); // for reshaping contiguous and distributed internally allocated memory
				void ReShape(NArraySupport::StorageOption storage_option, U*& value); // for reshaping contiguous externally allocated memory
				void ReSize(Index* index, const U& value); // for resizing all types of allocated memory - bottom-out the recursion (a non-member function called by PutSize kick-starts the recursion from the top)
				template <class S> void SerialiseNonUniform(S& in_out, std::fstream& ka, bool in, bool reindex);
				template <class S> void Write(S* values_array, int& position, const Mask* mask = nullptr) const; // need this function to bottom-out the recursion
				template <class S> void Write(S* values_array) const; // need this function if not recursing i.e. calling from a scalar
				void Write(std::ostream& output_stream, bool include_indexes = false) const;
	};
}
