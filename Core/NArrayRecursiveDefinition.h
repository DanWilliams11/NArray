// the functions in this file are for kick-starting and continuing the recursion, these functions are bottomed-out by functions in NArrayAxiomatic
// note that the functions called during recursion are not the same function, they are functions with the same signature belonging to classes templated at the next level down, eventually culminating in the functions in NArrayAxiomatic
// apart from a few recursive-specific functions, the functions in this file mirror those in NArrayAxiomatic

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
	template <class T, class U> class NArrayRecursive
	{
		// member variables
	private:
				T** m_data;
				bool m_external;
				int m_new_size;
				Shape m_shape;
				int m_size;
				U* m_values_array;
		// member functions
	private:
				void CreateNArrayRecursiveContiguousInternal(const Shape& shape, U*& values_array, const U& value, bool reindex); // constructor helper for contiguous internally allocated memory
				void CreateNArrayRecursiveContiguousExternal(const Shape& shape, U*& values_array, bool reindex); // constructor helper for contiguous externally allocated memory
				void CreateNArrayRecursiveDistributedInternal(const Shape& shape, const U& value, bool reindex); // constructor helper for distributed internally allocated memory
				void CreateNArrayRecursive(const NArrayRecursive& m); // copy constructor helper for all types of allocated memory during kick-start
				void CreateNArrayRecursive(const NArrayRecursive& m, int& position, U*& values_array); // copy constructor helper for all types of allocated memory during recursion
				void DestroyNArrayRecursive(); // destructor helper for all types of allocated memory
				Index* Initialise(const Shape& shape, Index& idx, U*& values_array, bool external, bool reindex); // constructor helper for all types of allocated memory during kick-start
				void Initialise(const Shape& shape, bool external); // constructor helper for all types of allocated memory during kick-start and recursion
				void Initialise(const NArrayRecursive& m); // copy constructor helper for all types of allocated memory
	public:
				NArrayRecursive(); // default constructor
				NArrayRecursive(NArraySupport::StorageOption storage_option, const Shape& shape, const U& value, bool reindex); // constructor (kick-starts the recursion) for contiguous and distributed internally allocated memory
				NArrayRecursive(NArraySupport::StorageOption storage_option, const Shape& shape, U*& values_array, bool reindex); // constructor (kick-starts the recursion) for contiguous externally allocated memory
				NArrayRecursive(const Shape& shape, Index* index, int& position, U*& values_array, const U& value); // recursive constructor (continues the recursion) for contiguous internally allocated memory
				NArrayRecursive(const Shape& shape, Index* index, int& position, U*& values_array); // recursive constructor (continues the recursion) for contiguous externally allocated memory
				NArrayRecursive(const Shape& shape, Index* index, const U& value); // recursive constructor (continues the recursion) for distributed internally allocated memory
				NArrayRecursive(const NArrayRecursive& m); // copy constructor (kick-starts the recursion) for all types of memory allocation
				NArrayRecursive(const NArrayRecursive& m, int& position, U*& values_array); // recursive copy constructor (continues the recursion) for all types of memory allocation
		virtual ~NArrayRecursive(); // destructor
				NArrayRecursive& operator =(const NArrayRecursive& m); // assignment operator
				T& operator [](int j) const; // member access operator
				template <class S, class V> bool ApplySingleThreadedOperator(const Mask& mask, SingleThreadedOperator<S, V>& op, const S& in, S& out, const ILogicalComparable<bool>* comparator = nullptr);
				template <class S1, class V1, class S2, class V2> bool ApplySingleThreadedTransform(const Mask& mask, SingleThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out, const ILogicalComparable<bool>* comparator = nullptr);
				template <class S, class V> void ApplyMultiThreadedOperator(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out);
				template <class S, class V> void ApplyMultiThreadedOperatorGo(const Mask& mask, MultiThreadedOperator<S, V>& op, const S& in, S& out);
				template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransform(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out);
				template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransformGo(const Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, const S1& in, S2& out);
				const T& GetData(int j) const; // need this function to kick-start and continue the recursion
				int GetDimSize(int dim) const; // see warning in cpp file
				int GetOrder() const;
				U* GetPointerToValue(const Index& index) const; // need this function to kick-start and continue the recursion - see warning in cpp file
				const Shape& GetShape() const; // see warning in cpp file
				int GetSize() const;
				int GetTotalSize() const; // need this function to kick-start and continue the recursion
				const U& GetValue(const Index& index) const; // need this function to kick-start and continue the recursion
				U GetWindowedValue(const Index& index) const; // need this function to kick-start and continue the recursion
				void PrettyPrint(std::ostream& output_stream, bool verbosity, const char* msg = nullptr, bool include_indexes = false) const;
				void PutData(int j, const T&); // need this function to kick-start and continue the recursion
				void PutPointerToValue(const Index& index, U* pointer_to_value); // need this function to kick-start and continue the recursion
				template <class S> void PutSize(S& in_out, int size, const U& value, bool reindex); // for resizing all types of allocated memory (this calls a non-member function to kick-start a recursive ReSize from the top) - see warning in cpp file
				void PutValue(const Index& index, const U& value); // need this function to kick-start and continue the recursion
				template <class S> void Read(const S* values_array, int& position, const Mask* mask = nullptr); // need this function to continue the recursion
				template <class S> void Read(const S* values_array, const Mask* mask = nullptr); // need this function to kick-start the recursion
				void Read(std::istream& input_stream);
				void ReShape(NArraySupport::StorageOption storage_option, const Shape& shape, const U& value, bool reindex); // for reshaping contiguous and distributed internally allocated memory
				void ReShape(NArraySupport::StorageOption storage_option, const Shape& shape, U*& values_array, bool reindex); // for reshaping contiguous externally allocated memory
				void ReSize(Index* index, const U& value); // for resizing all types of allocated memory - continue the recursion (a non-member function called by PutSize kick-starts the recursion from the top)
				template <class S> void SerialiseNonUniform(S& in_out, std::fstream& ka, bool in, bool reindex);
				void SerialiseUniform(NArraySupport::StorageOption storage_option, std::fstream& ka, bool in, bool reindex);
				void SwapValue(const Index& index1, const Index& index2);
				template <class S> void Write(S* values_array, int& position, const Mask* mask = nullptr) const; // need this function to continue the recursion
				template <class S> void Write(S* values_array, const Mask* mask = nullptr) const; // need this function to kick-start the recursion
				void Write(std::ostream& output_stream, bool include_indexes = false) const;
	};
}
