#pragma once

#include <type_traits>

#include "GL/freeglut.h"

#include "IVisualisable.h"
#include "VisualisationImplementation.h"

namespace NArray
{
	// abstract base class for visualising a subspace of an n-dimensional array - tested
	// this operator should not be multi-threaded because the OpenGL routines use the GPU
	template <class S, class V> class Visualiser abstract : public SingleThreadedOperator<S, V>
	{
		// member variables
	protected:
		double m_point_colour_blue;
		double m_point_colour_green;
		double m_point_colour_red;
		double m_point_size;
		NArraySupport::VisualisationMode m_visualisation_mode;
		static const int m_x_window_resolution = 1920; // TODO ask the system for this information
		double m_x_window_scale;
		int m_x_window_size;
		int m_x_window_start;
		static const int m_y_window_resolution = 1080; // TODO ask the system for this information
		double m_y_window_scale;
		int m_y_window_size;
		int m_y_window_start;
		double m_z_window_scale;

		// member functions
	public:
		Visualiser(NArraySupport::VisualisationMode visualisation_mode, double point_size, double point_colour_red, double point_colour_green, double point_colour_blue, int x_window_start, int y_window_start, int x_window_size, int y_window_size)
		{
			ReMode(visualisation_mode);
			ReSize(point_size);
			ReColour(point_colour_red, point_colour_green, point_colour_blue);
			ReWindow(x_window_start, y_window_start, x_window_size, y_window_size);
		}
		void EndDraw()
		{
			glEndList();
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
			glutMainLoop();
		}
		void PlotPoint()
		{
			glBegin(GL_POINTS);
			glVertex2d(0.0, 0.0);
			glEnd();
		}
		void PlotPoint(double x)
		{
			glBegin(GL_POINTS);
			glVertex2d(x, 0.0);
			glEnd();
		}
		void PlotPoint(double x, double y)
		{
			glBegin(GL_POINTS);
			glVertex2d(x, y);
			glEnd();
		}
		void PlotPoint(double x, double y, double z)
		{
			glBegin(GL_POINTS);
			glVertex3d(x, y, z);
			glEnd();
		}
		bool PreparePoint(double val)
		{
			if (val <= 0.0)
				return false;

			if (m_visualisation_mode == NArraySupport::UseDataToSize || m_visualisation_mode == NArraySupport::UseDataToSizeAndColour)
				glPointSize((GLfloat) val);
			else
				glPointSize((GLfloat) m_point_size);

			if (m_visualisation_mode == NArraySupport::UseDataToColour || m_visualisation_mode == NArraySupport::UseDataToSizeAndColour)
			{
				double point_colour_red = 0.0;
				double point_colour_green = 0.0;
				double point_colour_blue = 0.0;
				Visualisation<S, V>::ColourMap(val, point_colour_red, point_colour_green, point_colour_blue);
				glColor3d(point_colour_red, point_colour_green, point_colour_blue);
			}
			else
			{
				glColor3d(m_point_colour_red, m_point_colour_green, m_point_colour_blue);
			}

			return true;
		}
		void ReColour(double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0)
		{
			m_point_colour_red = point_colour_red;
			m_point_colour_green = point_colour_green;
			m_point_colour_blue = point_colour_blue;
		}
		void ReMode(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToColour)
		{
			m_visualisation_mode = visualisation_mode;
		}
		void ReSize(double point_size = 1.0)
		{
			m_point_size = point_size;
		}
		void ReWindow(int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
		{
			m_x_window_start = x_window_start;
			m_y_window_start = y_window_start;
			m_x_window_size = x_window_size;
			m_y_window_size = y_window_size;
			m_x_window_scale = (double) m_x_window_size / (double) m_x_window_resolution * 10.0;
			m_y_window_scale = (double) m_y_window_size / (double) m_y_window_resolution * 10.0;
			m_z_window_scale = 10.0;
		}
		void StartDraw()
		{
			Visualisation<S, V>::GetTheInstance().Initialise(m_visualisation_mode, m_x_window_start, m_y_window_start, m_x_window_size, m_y_window_size);
			glNewList(Visualisation<S, V>::Data, GL_COMPILE);
		}
	};

	// class for visualising a 0-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects do not inherit from IVisualisable
	template <class S, class V, bool = std::is_base_of<IVisualisable0D, V>::value> class Visualiser0D : public Visualiser<S, V>
	{
		// member functions
	public:
		Visualiser0D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Visualiser(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'

			if (PreparePoint(value))
				PlotPoint();
			else
				return false;

			return true;
		}
	};

	// partial specialisation of class for visualising a 0-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IVisualisable
	template <class S, class V> class Visualiser0D<S, V, true> : public Visualiser<S, V>
	{
		// member functions
	public:
		Visualiser0D(NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Visualiser(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'

			if (PreparePoint(value.GetScaledData()))
				PlotPoint();
			else
				return false;

			return true;
		}
	};

	// class for visualising a 1-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects do not inherit from IVisualisable
	template <class S, class V, bool = std::is_base_of<IVisualisable1D, V>::value> class Visualiser1D : public Visualiser0D<S, V>
	{
		// member variables
	protected:
		int m_x_dim;
		double m_x_size;
		// member functions
	public:
		Visualiser1D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000, int x_dim = 1)
			: Visualiser0D(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			ReShape(shape, x_dim);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;

			if (PreparePoint(value, index, x))
				PlotPoint(x);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, const Index& index, double& x)
		{
			if (!Visualiser0D::PreparePoint(val))
				return false;

			double x_pos = index.GetIndexAtCoord(m_x_dim);
			x = ((x_pos / m_x_size) - 0.5) * m_x_window_scale;

			return true;
		}
		virtual void ReShape(const Shape& shape, int x_dim = 1)
		{
			m_x_dim = x_dim;
			m_x_size = shape.GetShapeAtCoord(m_x_dim);
		}
	};

	// partial specialisation of class for visualising a 1-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IVisualisable
	template <class S, class V> class Visualiser1D<S, V, true> : public Visualiser0D<S, V>
	{
		// member functions
	public:
		Visualiser1D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Visualiser0D(visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
		{
			shape; // do this to prevent compiler warning 'unreferenced formal parameter'
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;

			if (PreparePoint(value, x))
				PlotPoint(x);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, double& x)
		{
			if (!Visualiser0D::PreparePoint(val.GetScaledData()))
				return false;

			x = (val.GetScaledX() - 0.5) * m_x_window_scale;

			return true;
		}
	};

	// class for visualising a 2-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects do not inherit from IVisualisable
	template <class S, class V, bool = std::is_base_of<IVisualisable2D, V>::value> class Visualiser2D : public Visualiser1D<S, V>
	{
		// member variables
	protected:
		int m_y_dim;
		double m_y_size;
		// member functions
	public:
		Visualiser2D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000, int x_dim = 1, int y_dim = 2)
			: Visualiser1D(shape, visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size, x_dim)
		{
			ReShape(shape, x_dim, y_dim);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;
			double y = 0.0;

			if (PreparePoint(value, index, x, y))
				PlotPoint(x, y);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, const Index& index, double& x, double& y)
		{
			if (!Visualiser1D::PreparePoint(val, index, x))
				return false;

			double y_pos = index.GetIndexAtCoord(m_y_dim);
			y = ((y_pos / m_y_size) - 0.5) * m_y_window_scale;

			return true;
		}
		virtual void ReShape(const Shape& shape, int x_dim = 1, int y_dim = 2)
		{
			Visualiser1D::ReShape(shape, x_dim);
			m_y_dim = y_dim;
			m_y_size = shape.GetShapeAtCoord(m_y_dim);
		}
	};

	// partial specialisation of class for visualising a 2-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IVisualisable
	template <class S, class V> class Visualiser2D<S, V, true> : public Visualiser1D<S, V>
	{
		// member functions
	public:
		Visualiser2D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Visualiser1D(shape, visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
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
				PlotPoint(x, y);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, double& x, double& y)
		{
			if (!Visualiser1D::PreparePoint(val, x))
				return false;

			y = (val.GetScaledY() - 0.5) * m_y_window_scale;

			return true;
		}
	};

	// class for visualising a 3-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects do not inherit from IVisualisable
	template <class S, class V, bool = std::is_base_of<IVisualisable3D, V>::value> class Visualiser3D : public Visualiser2D<S, V>
	{
		// member variables
	private:
		int m_z_dim;
		double m_z_size;
		// member functions
	public:
		Visualiser3D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000, int x_dim = 1, int y_dim = 2, int z_dim = 3)
			: Visualiser2D(shape, visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size, x_dim, y_dim)
		{
			ReShape(shape, x_dim, y_dim, z_dim);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;

			if (PreparePoint(value, index, x, y, z))
				PlotPoint(x, y, z);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, const Index& index, double& x, double& y, double& z)
		{
			if (!Visualiser2D::PreparePoint(val, index, x, y))
				return false;

			double z_pos = index.GetIndexAtCoord(m_z_dim);
			z = ((z_pos / m_z_size) - 0.5) * m_z_window_scale;

			return true;
		}
		virtual void ReShape(const Shape& shape, int x_dim = 1, int y_dim = 2, int z_dim = 3)
		{
			Visualiser2D::ReShape(shape, x_dim, y_dim);
			m_z_dim = z_dim;
			m_z_size = shape.GetShapeAtCoord(m_z_dim);
		}
	};

	// partial specialisation of class for visualising a 3-dimensional subspace of an n-dimensional array - tested
	// this will be used if the array objects inherit from IVisualisable
	template <class S, class V> class Visualiser3D<S, V, true> : public Visualiser2D<S, V>
	{
		// member functions
	public:
		Visualiser3D(const Shape& shape, NArraySupport::VisualisationMode visualisation_mode = NArraySupport::UseDataToSize, double point_size = 1.0, double point_colour_red = 1.0, double point_colour_green = 1.0, double point_colour_blue = 1.0, int x_window_start = 10, int y_window_start = 10, int x_window_size = 1000, int y_window_size = 1000)
			: Visualiser2D(shape, visualisation_mode, point_size, point_colour_red, point_colour_green, point_colour_blue, x_window_start, y_window_start, x_window_size, y_window_size)
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
				PlotPoint(x, y, z);
			else
				return false;

			return true;
		}
		bool PreparePoint(const V& val, double& x, double& y, double& z)
		{
			if (!Visualiser2D::PreparePoint(val, x, y))
				return false;

			z = (val.GetScaledZ() - 0.5) * m_z_window_scale;

			return true;
		}
	};
}
