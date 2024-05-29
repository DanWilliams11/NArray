#pragma once

#include "../MultiThreading/MultiThreading.h"
#include "../Testing/Timer.h"
#include "CombinationOperators.h"

namespace NArray
{
	// function for combining the values correspondingly across two n-dimensional arrays using the specified combiner to produce a new n-dimensional array - tested
	template <class S, class V> S CombineValues(S& in1, S& in2, const ICombinable<V>& combiner, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in1.GetShape() != in2.GetShape())
			throw(std::invalid_argument("NArray: <in1> and <in2> n-dimensional arrays must be of the same shape if their corresponding values are to be combined in function 'CombineValues'\n"));

		if (timer) timer->StartTimer();
		S out(in1);
		Combine<S, V> combine(combiner, out);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(combine, in1, in2, multi_threading_mask);
		else
			ApplySingleThreadedOperator(combine, in1, in2);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CombineValues' (correspondingly across two arrays) using the '" << combiner.GetName() << "' combiner took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
		return out;
	}
}
