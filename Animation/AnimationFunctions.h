#pragma once

#include "../Core/NArrayDefinition.h"
#include "../MultiThreading/MultiThreading.h"
#include "AnimationOperators.h"

namespace NArray
{
	// function for animating a 0-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Animate0D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Animator0D<S, V> animator_0D(visualisation_mode);
		ApplySingleThreadedOperator(animator_0D, in_out);
	}

	// function for animating a 1-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Animate1D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Animator1D<S, V> animator_1D(visualisation_mode);
		ApplySingleThreadedOperator(animator_1D, in_out);
	}

	// function for animating a 2-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Animate2D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Animator2D<S, V> animator_2D(visualisation_mode);
		ApplySingleThreadedOperator(animator_2D, in_out);
	}

	// function for animating a 3-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Animate3D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Animator3D<S, V> animator_3D(visualisation_mode);
		ApplySingleThreadedOperator(animator_3D, in_out);
	}
}
