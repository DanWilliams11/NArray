#pragma once

#include <atomic>
#include <chrono>
#include <thread>

#include "../Comparison/IComparable.h"
#include "../Core/Operator.h"
#include "../Core/NArraySupport.h"
#include "../Core/Transform.h"

namespace NArray
{
	// function for applying a multi-threaded operator to an n-dimensional array, writing the results to the same n-dimensional array - tested
	template <class S, class V> void ApplyMultiThreadedOperator(MultiThreadedOperator<S, V>& op, S& in, Mask* multi_threading_mask = nullptr)
	{
		ApplyMultiThreadedOperator(op, in, in, multi_threading_mask);
	}

	// function for applying a multi-threaded operator to an n-dimensional array, writing the results to another n-dimensional array of the same type - tested
	template <class S, class V> void ApplyMultiThreadedOperator(MultiThreadedOperator<S, V>& op, S& in, S& out, Mask* multi_threading_mask = nullptr)
	{
		if (multi_threading_mask)
			op.AddMask(*multi_threading_mask);
		else
			op.UseMask(false);
		Mask mask(in.GetOrder());
		mask.Initialise();
		in.ApplyMultiThreadedOperator(mask, op, in, out);
		while (NArraySupport::MultiThreading::GetThreadCount() > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		NArraySupport::MultiThreading::JoinThreads();
	}

	// function for applying a non-blocking multi-threaded operator to an n-dimensional array, writing the results to the same n-dimensional array - tested
	// since non-blocking, have to pass mask in from outside, otherwise it will die when function returns
	template <class S, class V> void ApplyMultiThreadedOperatorNonBlocking(Mask& mask, MultiThreadedOperator<S, V>& op, S& in, Mask* multi_threading_mask = nullptr)
	{
		ApplyMultiThreadedOperatorNonBlocking(mask, op, in, in, multi_threading_mask);
	}

	// function for applying a non-blocking multi-threaded operator to an n-dimensional array, writing the results to another n-dimensional array of the same type - tested
	// since non-blocking, have to pass mask in from outside, otherwise it will die when function returns
	template <class S, class V> void ApplyMultiThreadedOperatorNonBlocking(Mask& mask, MultiThreadedOperator<S, V>& op, S& in, S& out, Mask* multi_threading_mask = nullptr)
	{
		if (multi_threading_mask)
			op.AddMask(*multi_threading_mask);
		else
			op.UseMask(false);
		in.ApplyMultiThreadedOperator(mask, op, in, out);
	}

	// function for applying a multi-threaded transform to an n-dimensional array of one type, writing the results to another n-dimensional array of a different type - tested
	template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransform(MultiThreadedTransform<S1, V1, S2, V2>& tr, S1& in, S2& out, Mask* multi_threading_mask = nullptr)
	{
		if (multi_threading_mask)
			tr.AddMask(*multi_threading_mask);
		else
			tr.UseMask(false);
		Mask mask(in.GetOrder());
		mask.Initialise();
		in.ApplyMultiThreadedTransform(mask, tr, in, out);
		while (NArraySupport::MultiThreading::GetThreadCount() > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		NArraySupport::MultiThreading::JoinThreads();
	}

	// function for applying a non-blocking multi-threaded transform to an n-dimensional array of one type, writing the results to another n-dimensional array of a different type - tested
	// since non-blocking, have to pass mask in from outside, otherwise it will die when function returns
	template <class S1, class V1, class S2, class V2> void ApplyMultiThreadedTransformNonBlocking(Mask& mask, MultiThreadedTransform<S1, V1, S2, V2>& tr, S1& in, S2& out, Mask* multi_threading_mask = nullptr)
	{
		if (multi_threading_mask)
			tr.AddMask(*multi_threading_mask);
		else
			tr.UseMask(false);
		in.ApplyMultiThreadedTransform(mask, tr, in, out);
	}

	// function for applying a single-threaded operator to an n-dimensional array, writing the results to the same n-dimensional array - tested
	template <class S, class V> bool ApplySingleThreadedOperator(SingleThreadedOperator<S, V>& op, S& in, const ILogicalComparable<bool>* comparator = nullptr)
	{
		return ApplySingleThreadedOperator(op, in, in, comparator);
	}

	// function for applying a single-threaded operator to an n-dimensional array, writing the results to another n-dimensional array of the same type - tested
	template <class S, class V> bool ApplySingleThreadedOperator(SingleThreadedOperator<S, V>& op, S& in, S& out, const ILogicalComparable<bool>* comparator = nullptr)
	{
		Mask mask(in.GetOrder());
		mask.Initialise();
		return in.ApplySingleThreadedOperator(mask, op, in, out, comparator);
	}

	// function for applying a single-threaded transform to an n-dimensional array of one type, writing the results to another n-dimensional array of a different type - tested
	template <class S1, class V1, class S2, class V2> bool ApplySingleThreadedTransform(SingleThreadedTransform<S1, V1, S2, V2>& tr, S1& in, S2& out, const ILogicalComparable<bool>* comparator = nullptr)
	{
		Mask mask(in.GetOrder());
		mask.Initialise();
		return in.ApplySingleThreadedTransform(mask, tr, in, out, comparator);
	}
}
