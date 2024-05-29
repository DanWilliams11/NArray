/*
Utility code for n-dimensional arrays of arbitrary shape and data type.

At the point of construction, the array will always be uniform.

After construction, individual elements of the array can be re-sized.
The original information is preserved, but contiguity (if applicable) is not (any new elements required during stretching will be added as distributed).
The original shape will no longer reflect the array as a whole and the array is now referred to as non-uniform.

After construction, the entire array can be re-shaped.
The original information is not preserved, but contiguity (if applicable) is.
The new shape will reflect the array as a whole and uniformity is preserved.

For mixed data types, where each element of the array can have a different data type, use NArray::GenericType.

Note:
NArray indexing starts at one, not zero.
Internally, it starts at zero, but from a user's perspective, it starts at one, as this was felt to be more natural.
For example, the first row of a matrix is row 1, not row 0.

The n-dimensional arrays are defined recursively through templating:

Axiomatic template class for dimension 0: NArrayAxiomatic<<data_type>>
Recursive template class for dimension n: NArrayRecursive<NArrayRecursive, <data_type>>, <data_type>>

Example for dimension 3 and a data type of double:

NArrayRecursive<NArrayRecursive<NArrayRecursive<NArrayAxiomatic<double>, double>, double>, double>

It is not expected that the user will create arrays in this way.
Rather, they will use the parameterised typedefs that have been created for the first twenty dimensions:

Dimension	Use									Or Use
0			NArrayType<<data_type>>::M0		NArrayType<<data_type>>::Scalar
1			NArrayType<<data_type>>::M1		NArrayType<<data_type>>::Vector
2			NArrayType<<data_type>>::M2		NArrayType<<data_type>>::Matrix
3			NArrayType<<data_type>>::M3		NArrayType<<data_type>>::Array3D
...			...									...
20			NArrayType<<data_type>>::M20	NArrayType<<data_type>>::Array20D

The parameterised typedefs can easily be extended for higher dimensions if required.

There are three storage options, specified by the first argument to the constructor:

1. Contiguous internally allocated:
Use 'NArrayType<<data_type>>::Scalar <array_name>(ContigiousInternal, <initial_value>)' for scalars.
Use 'NArrayType<<data_type>>::Vector <array_name>(ContigiousInternal, <shape>, <initial_value>)' for vectors.
...
Use 'NArrayType<<data_type>>::M<n> <array_name>(ContigiousInternal, <shape>, <initial_value>)' for higher dimensions.

2. Contiguous externally allocated:
Use 'NArrayType<<data_type>>::Scalar <array_name>(ContigiousExternal, <values_array>)' for scalars.
Use 'NArrayType<<data_type>>::Vector <array_name>(ContigiousExternal, <shape>, <values_array>)' for vectors.
...
Use 'NArrayType<<data_type>>::M<n> <array_name>(ContigiousExternal, <shape>, <values_array>)' for higher dimensions.

3. Distributed internally allocated:
Use 'NArrayType<<data_type>>::Scalar <array_name>(DistributedInternal, <initial_value>)' for scalars.
Use 'NArrayType<<data_type>>::Vector <array_name>(DistributedInternal, <shape>, <initial_value>)' for vectors.
...
Use 'NArrayType<<data_type>>::M<n> <array_name>(DistributedInternal, <shape>, <initial_value>)' for higher dimensions.

<data_type>, <n>, <array_name>, <shape>, <values_array> and <initial_value> are to be supplied by the user.
<data_type> is the data type required - bool, char, int, double, std::complex<double>, std::string, NArray::GenericType etc.
<n> is the number of dimensions required - 0 for scalars, 1 for vectors, 2 for matrices etc.
As an alternative to M<n>, use Scalar, Vector, Matrix, Array3D etc.
<shape> is the shape of the array (not applicable to scalars).
<values_array> contains the data to be stored in the array for contiguous externally allocated storage.
<initial_value> is the value to initialise the array with for contiguous and distributed internally allocated storage.

The contiguous options allow a single block of data to be allocated for the entire array.
This is good for performance, but for very large arrays, there may not be a single block large enough.
In this case, the distributed option can be used, where each element in the array allocates just the memory it needs.
This is good for utilising already fragmented memory, but may be slower.
A future development is envisioned that mixes these two approaches.
The largest single block would be obtained as contiguous, the rest as distributed.
This is what happens anyway when an array is re-sized (not re-shaped) to be bigger than the original array.
In the case of re-sizing, the array is stretched - information is preserved, but contiguity is not preserved.
In the case of re-shaping, the entire array is re-constructed - information is not preserved, but contiguity is preserved.
In the case of externally allocated memory, allocation/de-allocation is not controlled by the array.
This is useful if the lifetime of the data for the array is longer than that of the array itself.

Note:
The chosen data type must implement a copy constructor and an assignment operator.
Serialisation is supported through the Read and Write functions.
The chosen data type must implement the << and >> operators for the chosen input/output classes e.g. std::fstream.
If the paramaterised typedefs are extended, the 'NArrayRecursive::GetOrder' function will also need to be extended.
This is simple to do and is fully explained within the comments for the function.
Furthermore, if it is not done, an explanatory exception will be thrown if the new paramaterised typedef is used.
*/

#pragma once

#include "NArrayAxiomaticDefinition.h"
#include "NArrayRecursiveDefinition.h"

namespace NArray
{
	// constants
	const int c_scalar_size = 1;
	const int c_scalar_order = 0;
	const int c_vector_order = 1;
	const int c_matrix_order = 2;
	const int c_array3D_order = 3;
	const int c_array4D_order = 4;
	const int c_array5D_order = 5;
	const int c_array6D_order = 6;
	const int c_array7D_order = 7;
	const int c_array8D_order = 8;
	const int c_array9D_order = 9;
	const int c_array10D_order = 10;
	const int c_array11D_order = 11;
	const int c_array12D_order = 12;
	const int c_array13D_order = 13;
	const int c_array14D_order = 14;
	const int c_array15D_order = 15;
	const int c_array16D_order = 16;
	const int c_array17D_order = 17;
	const int c_array18D_order = 18;
	const int c_array19D_order = 19;
	const int c_array20D_order = 20;
	// TODO extend here for higher dimensions as follows:
	// const int c_array<n>D_order = <n>;

	// typedefs
	template <class U> struct NArrayType
	{
		typedef NArrayAxiomatic<U> M0, Scalar;
		typedef NArrayRecursive<M0, U> M1, Vector;
		typedef NArrayRecursive<M1, U> M2, Matrix;
		typedef NArrayRecursive<M2, U> M3, Array3D;
		typedef NArrayRecursive<M3, U> M4, Array4D;
		typedef NArrayRecursive<M4, U> M5, Array5D;
		typedef NArrayRecursive<M5, U> M6, Array6D;
		typedef NArrayRecursive<M6, U> M7, Array7D;
		typedef NArrayRecursive<M7, U> M8, Array8D;
		typedef NArrayRecursive<M8, U> M9, Array9D;
		typedef NArrayRecursive<M9, U> M10, Array10D;
		typedef NArrayRecursive<M10, U> M11, Array11D;
		typedef NArrayRecursive<M11, U> M12, Array12D;
		typedef NArrayRecursive<M12, U> M13, Array13D;
		typedef NArrayRecursive<M13, U> M14, Array14D;
		typedef NArrayRecursive<M14, U> M15, Array15D;
		typedef NArrayRecursive<M15, U> M16, Array16D;
		typedef NArrayRecursive<M16, U> M17, Array17D;
		typedef NArrayRecursive<M17, U> M18, Array18D;
		typedef NArrayRecursive<M18, U> M19, Array19D;
		typedef NArrayRecursive<M19, U> M20, Array20D;
		// TODO extend here for higher dimensions as follows:
		// typedef NArrayRecursive<M<n>, U> M<n+1>, Array<n+1>D;
	};
}
