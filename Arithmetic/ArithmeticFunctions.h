#pragma once

#include <fstream>

#include "ArithmeticOperators.h"
#include "ArithmeticTransforms.h"
#include "../Statistical/StatisticalFunctions.h"
#include "../Testing/Timer.h"

namespace NArray
{
	// function for transforming an n-dimensional array of complex numbers to an n-dimensional array of their arguments - tested
	template <class S1, class V1, class S2, class V2> void CalculateArguments(S1& in, S2& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the arguments of the values in <in> in function 'CalculateArguments'\n"));

		if (timer) timer->StartTimer();
		Argument<S1, V1, S2, V2> argument;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(argument, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(argument, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateArguments' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for convolving an n-dimensional array with an n-dimensional kernel to produce another n-dimensional array - tested
	template <class S, class V> void CalculateConvolution(S& in, S& kernel, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the convolution of <in> with <kernel> in function 'CalculateConvolution'\n"));

		if (timer) timer->StartTimer();
		Convolution<S, V> convolution(kernel);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(convolution, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(convolution, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateConvolution' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for transforming a matrix of complex numbers to a matrix of their Julia Set - tested
	template <class S1, class V1, class S2, class V2> void CalculateJuliaSet(S1& in, S2& out, V1 seed, Mask* multi_threading_mask = nullptr)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Julia Set of <in> in function 'CalculateJuliaSet'\n"));

		JuliaSet<S1, V1, S2, V2> julia_set(seed);
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(julia_set, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(julia_set, in, out);
	}

	// function for transforming a matrix of complex numbers to a matrix of their Mandelbrot Set - tested
	template <class S1, class V1, class S2, class V2> void CalculateMandelbrotSet(S1& in, S2& out, Mask* multi_threading_mask = nullptr)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Mandelbrot Set of <in> in function 'CalculateMandelbrotSet'\n"));

		MandelbrotSet<S1, V1, S2, V2> mandelbrot_set;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(mandelbrot_set, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(mandelbrot_set, in, out);
	}

	// function for transforming an n-dimensional array of complex numbers to an n-dimensional array of their moduli - tested
	template <class S1, class V1, class S2, class V2> void CalculateModuli(S1& in, S2& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the moduli of the values in <in> in function 'CalculateModuli'\n"));

		if (timer) timer->StartTimer();
		Modulus<S1, V1, S2, V2> modulus;
		if (multi_threading_mask)
			ApplyMultiThreadedTransform(modulus, in, out, multi_threading_mask);
		else
			ApplySingleThreadedTransform(modulus, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateModuli' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for multiplying the values in an n-dimensional array together to generate a single value for the entire array - tested
	template <class S, class V> V CalculatePi(S& in, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (timer) timer->StartTimer();
		Pi<S, V> pi;
		ApplySingleThreadedOperator(pi, in);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculatePi' took " << timer->GetTime() << std::endl;
			*output_file << "Result: " << pi.GetPi() << std::endl;
		}
		return pi.GetPi();
	}

	// function for adding the values in an n-dimensional array together to generate a single value for the entire array - tested
	template <class S, class V> V CalculateSigma(S& in, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (timer) timer->StartTimer();
		Sigma<S, V> sigma;
		ApplySingleThreadedOperator(sigma, in);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateSigma' took " << timer->GetTime() << std::endl;
			*output_file << "Result: " << sigma.GetSigma() << std::endl;
		}
		return sigma.GetSigma();
	}

	// function for mapping the values in an n-dimensional array to occupy the values between the specified new_min_value and new_max_value - tested
	template <class S, class V> void MapValues(S& in_out, V new_min_value, V new_max_value, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		V current_min_value;
		Index current_min_index;
		V current_max_value;
		Index current_max_index;
		CalculateExtrema(in_out, current_min_value, current_min_index, current_max_value, current_max_index);
		V offset = -current_min_value;
		OffsetValues(in_out, offset, multi_threading_mask);
		if (current_max_value != current_min_value)
		{
			V scale = (new_max_value - new_min_value) / (current_max_value - current_min_value);
			ScaleValues(in_out, scale, multi_threading_mask);
		}
		offset = new_min_value;
		OffsetValues(in_out, offset, multi_threading_mask);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'MapValues' took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for offsetting the values in an n-dimensional array by one (the multiplicative identity element) - tested
	template <class S, class V> void OffsetValues(S& in_out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		Offset<S, V> offset;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(offset, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(offset, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'OffsetValues' (offset by the multiplicative identity element) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for offsetting the values in an n-dimensional array by the specified offset - tested
	template <class S, class V> void OffsetValues(S& in_out, V offst, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		Offset<S, V> offset(offst);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(offset, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(offset, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'OffsetValues' (offset by the specified offst) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for scaling the values in an n-dimensional array by the specified scale - tested
	template <class S, class V> void ScaleValues(S& in_out, V scl, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (timer) timer->StartTimer();
		Scale<S, V> scale(scl);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(scale, in_out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(scale, in_out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'ScaleValues' (scale by the specified scl) took " << timer->GetTime() << std::endl;
			in_out.PrettyPrint(*output_file, verbosity);
		}
	}
}
