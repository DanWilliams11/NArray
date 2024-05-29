#pragma once

#include "../MultiThreading/MultiThreading.h"
#include "CommunicationOperators.h"

namespace NArray
{
	// function for communicating between the objects in an n-dimensional array - tested
	template <class S, class V> void CommunicateValues(S& in_out)
	{
		Mask multi_threading_mask(in_out.GetOrder());
		multi_threading_mask.Initialise();
		multi_threading_mask.PutMaskAtDim(0, Mask::use_all);
		Mask* multi_threading_mask_ptr = &multi_threading_mask;

		ICommunicable::SetContinue(true);

		Mask mask(in_out.GetOrder());
		mask.Initialise();
		Communicate<S, V> communicate;
		ApplyMultiThreadedOperatorNonBlocking<S, V>(mask, communicate, in_out, multi_threading_mask_ptr);

		Sleep(5000);
		ICommunicable::SetContinue(false);
		Sleep(5000);
	}
}
