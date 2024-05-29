#pragma once

#include "../Arithmetic/ArithmeticFunctions.h"
#include "../Core/NArrayDefinition.h"
#include "VisualisationOperators.h"

namespace NArray
{
	// function for visualising a 0-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Visualise0D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Visualiser0D<S, V> visualiser_0D(visualisation_mode);
		visualiser_0D.StartDraw();
		ApplySingleThreadedOperator(visualiser_0D, in_out);
		visualiser_0D.EndDraw();
	}

	// function for visualising a 1-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Visualise1D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Visualiser1D<S, V> visualiser_1D(in_out.GetShape(), visualisation_mode);
		visualiser_1D.StartDraw();
		ApplySingleThreadedOperator(visualiser_1D, in_out);
		visualiser_1D.EndDraw();
	}

	// function for visualising a 2-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Visualise2D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Visualiser2D<S, V> visualiser_2D(in_out.GetShape(), visualisation_mode);
		visualiser_2D.StartDraw();
		ApplySingleThreadedOperator(visualiser_2D, in_out);
		visualiser_2D.EndDraw();
	}

	// function for visualising a 3-dimensional subspace of an n-dimensional array - tested
	template <class S, class V> void Visualise3D(S& in_out, NArraySupport::VisualisationMode visualisation_mode)
	{
		Visualiser3D<S, V> visualiser_3D(in_out.GetShape(), visualisation_mode);
		visualiser_3D.StartDraw();
		ApplySingleThreadedOperator(visualiser_3D, in_out);
		visualiser_3D.EndDraw();
	}
}
