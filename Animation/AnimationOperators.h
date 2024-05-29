#pragma once

#include "AnimationImplementation.h"
#include "IAnimatable.h"
#include "../Visualisation/VisualisationOperators.h"

namespace NArray
{
	// abstract base class for animating a subspace of an n-dimensional array - tested
	// this operator should not be multi-threaded because the OpenGL routines use the GPU
	template <class S, class V> class Animator abstract : public Visualiser<S, V>
	{
		// member functions
	public:
		Animator(NArraySupport::VisualisationMode visualisation_mode, double point_size, double point_colour_red, double point_colour_green, double point_colour_blue, int x_window_start, int y_window_start, int x_window_size, int y_window_size)
			: Visualiser(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initaliser list
		}
		void EndDraw()
		{
			// hide the functionality inherited from Visualiser
			// TODO throw an exception if called?
		}
		bool PreparePoint(const V& val)
		{
			if (m_visualisation_mode == NArraySupport::UseDataToSize || m_visualisation_mode == NArraySupport::UseDataToSizeAndColour)
				glPointSize((GLfloat) val.GetScaledData());
			else
				glPointSize((GLfloat) m_point_size);

			if (m_visualisation_mode == NArraySupport::UseDataToColour || m_visualisation_mode == NArraySupport::UseDataToSizeAndColour)
			{
				double point_colour_red = 0.0;
				double point_colour_green = 0.0;
				double point_colour_blue = 0.0;
				Animation<S, V>::ColourMap(val.GetColour(), point_colour_red, point_colour_green, point_colour_blue);
				glColor3d(point_colour_red, point_colour_green, point_colour_blue);
			}
			else
			{
				glColor3d(m_point_colour_red, m_point_colour_green, m_point_colour_blue);
			}

			return true;
		}
		void StartDraw()
		{
			// hide the functionality inherited from Visualiser
			// TODO throw an exception if called?
		}
	};

	// class for animating a 0-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IAnimatable
	template <class S, class V> class Animator0D : public Animator<S, V>
	{
		// member functions
	public:
		Animator0D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Animator(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'

			if (PreparePoint(value))
			{
				PlotPoint();
				value.Update();
			}
			else
			{
				return false;
			}

			return true;
		}
	};

	// class for animating a 1-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IAnimatable
	template <class S, class V> class Animator1D : public Animator0D<S, V>
	{
		// member functions
	public:
		Animator1D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Animator0D(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;

			if (PreparePoint(value, x))
			{
				PlotPoint(x);
				value.Update();
			}
			else
			{
				return false;
			}

			return true;
		}
		bool PreparePoint(const V& val, double& x)
		{
			if (!Animator0D::PreparePoint(val))
				return false;

			x = val.GetScaledX();

			return true;
		}
	};

	// class for animating a 2-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IAnimatable
	template <class S, class V> class Animator2D : public Animator1D<S, V>
	{
		// member functions
	public:
		Animator2D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Animator1D(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;
			double y = 0.0;

			if (PreparePoint(value, x, y))
			{
				PlotPoint(x, y);
				value.Update();
			}
			else
			{
				return false;
			}

			return true;
		}
		bool PreparePoint(const V& val, double& x, double& y)
		{
			if (!Animator1D::PreparePoint(val, x))
				return false;

			y = val.GetScaledY();

			return true;
		}
	};

	// class for animating a 3-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IAnimatable
	template <class S, class V> class Animator3D : public Animator2D<S, V>
	{
		// member functions
	public:
		Animator3D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Animator2D(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;
			
			if (PreparePoint(value, x, y, z))
			{
				PlotPoint(x, y, z);
				value.Update();
			}
			else
			{
				return false;
			}

			return true;
		}
		bool PreparePoint(const V& val, double& x, double& y, double& z)
		{
			if (!Animator2D::PreparePoint(val, x, y))
				return false;

			z = val.GetScaledZ();

			return true;
		}
	};
}
