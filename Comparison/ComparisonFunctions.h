#pragma once

#include "../MultiThreading/MultiThreading.h"
#include "../Testing/Timer.h"
#include "../Useful/UsefulOperators.h"
#include "ComparisonOperators.h"
#include "LogicalComparators.h"

namespace NArray
{
	// function for comparing the values correspondingly across two n-dimensional arrays using the specified comparator - tested
	template <class S, class V> bool CompareValues(S& in1, S& in2, const IComparable<V>& comparator, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (in1.GetShape() != in2.GetShape())
			throw(std::invalid_argument("NArray: <in1> and <in2> n-dimensional arrays must be of the same shape if their corresponding values are to be compared in function 'CompareValues'\n"));

		if (timer) timer->StartTimer();
		And<bool> and;
		Compare<S, V> compare(comparator);
		bool res = ApplySingleThreadedOperator(compare, in1, in2, &and);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CompareValues' (correspondingly across two arrays) using the '" << comparator.GetName() << "' comparator took " << timer->GetTime() << std::endl;
			*output_file << "Result: " << res << std::endl;
		}
		return res;
	}

	// function for comparing the values sequentially within one n-dimensional array using the specified comparator - tested
	// Note: The result of this function depends on the order of the values in the array
	template <class S, class V> bool CompareValues(S& in, const ILogicalComparable<V>& comparator, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (timer) timer->StartTimer();
		GetValues<S, V> get_values;
		bool res = ApplySingleThreadedOperator(get_values, in, &comparator);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CompareValues' (sequentially within one array) using the '" << comparator.GetName() << "' comparator took " << timer->GetTime() << std::endl;
			*output_file << "Result: " << res << std::endl;
		}
		return res;
	}
}
