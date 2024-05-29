#pragma once

#include "../Comparison/RelationalComparators.h"
#include "../Composition/UsefulCompositions.h"
#include "../Core/NArrayImplementation.h"
#include "../MultiThreading/MultiThreading.h"
#include "../Testing/Timer.h"
#include "UsefulOperators.h"
#include "UsefulTransforms.h"

namespace NArray
{
	// function for calculating the absolute differences in row values between all pairwise combinations of columns in a matrix, writing them to the rows of columns in a second matrix - tested
	// TODO can this be multi-threaded?
	template <class S1, class V1, class S2, class V2> S1 DiffPairwiseColumns(S1& in, NArraySupport::StorageOption out_storage_option = NArraySupport::ContiguousInternal, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Shape shape = in.GetShape();
		if (shape.GetOrder() != c_matrix_order)
			throw(std::invalid_argument("NArray: <in> n-dimensional array must be a matrix in function 'DiffPairwiseColumns'\n"));

		if (timer) timer->StartTimer();
		int num_rows = shape.GetShapeAtCoord(1);
		int num_cols = shape.GetShapeAtCoord(2);
		int num_pairwise_cols = ((num_cols * num_cols) - num_cols) / 2;
		shape.ReShape(c_matrix_order, num_rows, num_pairwise_cols);
		S1 out(out_storage_option, shape, V1(), false);
		shape.ReShape(c_matrix_order, num_cols, num_cols);
		S2 pairwise_cols(NArraySupport::ContiguousInternal, shape, V2(), true);
		MakeTriangleOne<S2, V2>(pairwise_cols, NArraySupport::UTR);
		Mask mask(c_matrix_order, Mask::use_all, Mask::use_all);
		DiffAllColumns<S1, V1, S2, V2> diff_all_columns(pairwise_cols);
		pairwise_cols.ApplySingleThreadedTransform(mask, diff_all_columns, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'DiffPairwiseColumns' (calculate the absolute differences in row values between all pairwise combinations of columns in the array to create a new array) took " << timer->GetTime() << std::endl;
			in.PrettyPrint(*output_file, verbosity, "Input array:");
			out.PrettyPrint(*output_file, verbosity, "Output array:");
		}
		return out;
	}

	// function for transforming the pointer types of the values in an n-dimensional array of one type to an n-dimensional array of different pointer types in the same inheritance hierarchy (upcast / downcast) - tested
	template <class S1, class V1, class S2, class V2> void DynamicCastValues(S1& in, S2& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the cast of the values in <in> in function 'DynamicCastValues'\n"));

		if (timer) timer->StartTimer();
		DynamicCast<S1, V1, S2, V2> cast;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(cast, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(cast, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'DynamicCastValues' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in a matrix of std::complex<double> so as to create the Complex Plane (ramped along the real axis, ramped along the imaginary axis) - tested
	template <class S, class V> void MakeComplexPlane(S& in_out, V start_val, V step_val, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in_out.GetShape().GetOrder() != c_matrix_order)
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be a matrix in function 'MakeComplexPlane'\n"));

		V v1;
		std::complex<double> v2;
		if (typeid(v1) != typeid(v2))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be a matrix of std::complex<double> in function 'MakeComplexPlane'\n"));

		if (timer) timer->StartTimer();
		SetToComplexPlane<S, V> set_to_complex_plane(start_val, step_val);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_complex_plane, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_complex_plane, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeComplexPlane' took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for making a square n-dimensional array the identity n-dimensional array - tested
	template <class S, class V> void MakeIdentity(S& in_out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to be made the identity n-dimensional array in function 'MakeIdentity>\n"));

		if (timer) timer->StartTimer();
		SetToIdentity<S, V> set_to_identity;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_identity, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_identity, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeIdentity' (make the array the identity array (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to one (the multiplicative identity element) - tested
	template <class S, class V> void MakeOne(S& in_out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToOne<S, V> set_to_one;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_one, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_one, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeOne' (set values to the multiplicative identity element) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to be ramped (monotonically increasing or decreasing), starting at zero (the additive identity element) and increasing by one (the multiplicative identity element) - tested
	template <class S, class V> void MakeRamped(S& in_out, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToRamped<S, V> set_to_ramped;
		ApplySingleThreadedOperator(set_to_ramped, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeRamped' (start at the additive identity element and increment by the multiplicative identity element) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to be ramped (monotonically increasing or decreasing), starting at the specified start_val and changing by the specified step_val - tested
	template <class S, class V> void MakeRamped(S& in_out, V start_val, V step_val, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToRamped<S, V> set_to_ramped(start_val, step_val);
		ApplySingleThreadedOperator(set_to_ramped, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeRamped' (start at the specified start_val and increment by the specified step_val) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to randomly generated values between zero (the additive identity element) and one (the multiplicative identity element) using the rand generator - tested
	template <class S, class V> void MakeRandom(S& in_out, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToRandom<S, V> set_to_random;
		ApplySingleThreadedOperator(set_to_random, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeRandom' (randomise the values between the additive identity element and the multiplicative identity element) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to randomly generated values between the specified min and max using the rand generator - tested
	template <class S, class V> void MakeRandom(S& in_out, V min, V max, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToRandom<S, V> set_to_random(min, max);
		ApplySingleThreadedOperator(set_to_random, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeRandom' (randomise the values between the specified min and the specified max) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for making a square n-dimensional array symmetric using its LTR or UTR - tested
	template <class S, class V> void MakeSymmetric(S& in_out, NArraySupport::RegionSpecifier region_specifier, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to made symmetric using its LTR or UTR in function 'MakeSymmetric'\n"));

		if (timer) timer->StartTimer();
		SetToSymmetric<S, V> set_to_symmetric(region_specifier);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_symmetric, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_symmetric, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeSymmetric' with the specified region_specifier (make the array symmetric using its lower-triangle or upper-triangle (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for making a square n-dimensional array the transpose of itself - tested
	template <class S, class V> void MakeTranspose(S& in_out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to be made the transpose of itself in function 'MakeTranspose'\n"));

		if (timer) timer->StartTimer();
		SetToTranspose<S, V> set_to_transpose;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_transpose, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_transpose, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeTranspose' (make the array the transpose of itself (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the LTR, UTR or DIAG of a square n-dimensional array to one (the multiplicative identity element) - tested
	template <class S, class V> void MakeTriangleOne(S& in_out, NArraySupport::RegionSpecifier region_specifier, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to have an LTR, UTR or DIAG in function 'MakeTriangleOne'\n"));

		if (timer) timer->StartTimer();
		SetTriangleToOne<S, V> set_triangle_to_one(region_specifier);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_triangle_to_one, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_triangle_to_one, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeTriangleOne' with the specified region_specifier (make the array's upper-triangle, lower-triangle or diagonal the multiplicative identity element (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the LTR, UTR or DIAG of a square n-dimensional array to the same value - tested
	template <class S, class V> void MakeTriangleValue(S& in_out, NArraySupport::RegionSpecifier region_specifier, V value, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to have an LTR, UTR or DIAG in function 'MakeTriangleValue'\n"));

		if (timer) timer->StartTimer();
		SetTriangleToValue<S, V> set_triangle_to_value(region_specifier, value);
		ApplySingleThreadedOperator(set_triangle_to_value, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeTriangleValue' with the specified region_specifier and value (make the array's upper_triangle, lower_triangle or diagonal equal to the specified value (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the LTR, UTR or DIAG of a square n-dimensional array to zero (the additive identity element) - tested
	template <class S, class V> void MakeTriangleZero(S& in_out, NArraySupport::RegionSpecifier region_specifier, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		Equal<int> equal;
		if (!NArraySupport::QueryShape(in_out.GetShape(), equal))
			throw(std::invalid_argument("NArray: <in_out> n-dimensional array must be square if it is to have an LTR, UTR or DIAG in function 'MakeTriangleZero'\n"));

		if (timer) timer->StartTimer();
		SetTriangleToZero<S, V> set_triangle_to_zero(region_specifier);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_triangle_to_zero, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_triangle_to_zero, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeTriangleZero' with the speciifed region_specifier (make the array's upper_triangle, lower-triangle or diagonal the additive identity element (array must be square for this otherwise an exception is thrown)) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to the same value - tested
	template <class S, class V> void MakeValue(S& in_out, V value, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToValue<S, V> set_to_value(value);
		ApplySingleThreadedOperator(set_to_value, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeValue' (set values to the specified value) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for setting the values in an n-dimensional array to zero (the additive identity element) - tested
	template <class S, class V> void MakeZero(S& in_out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		SetToZero<S, V> set_to_zero;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(set_to_zero, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(set_to_zero, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MakeZero' (set values to the additive identity element) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for populating the objects in one n-dimensional array using objects from another n-dimensional array - tested
	// the objects in the receiving array must inherit from IPopulatable
	template <class S1, class V1, class S2, class V2> void PopulateValues(S1& in, S2& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be populated with the values from <in> in function 'PopulateValues'\n"));

		if (timer) timer->StartTimer();
		Populate<S1, V1, S2, V2> populate;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(populate, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(populate, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'PopulateValues' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for transforming an n-dimensional array of one type to an n-dimensional array of a different type - tested
	template <class S1, class V1, class S2, class V2> void StaticCastValues(S1& in, S2& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the cast of the values in <in> in function 'StaticCastValues'\n"));

		if (timer) timer->StartTimer();
		StaticCast<S1, V1, S2, V2> cast;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(cast, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(cast, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'StaticCastValues' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for transposing an n-dimensional array to generate a new n-dimensional array - tested
	template <class S, class V> S Transpose(S& in, Mask* multi_threading_mask = nullptr, NArraySupport::StorageOption out_storage_option = NArraySupport::ContiguousInternal, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		V v;
		S out(out_storage_option, in.GetShape().Transpose(), v, false);
		CopyToTranspose<S, V> copy_to_transpose;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(copy_to_transpose, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(copy_to_transpose, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'Transpose' (transpose the array to create a new array (array need not be square for this)) took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
		return out;
	}
}
