#pragma once

#include <fstream>

#include "GL/freeglut.h"

#include "VisualisationDefinition.h"

using namespace NArray;

template <class S, class V> Visualisation<S, V>::Visualisation()
{
	m_current_x = 0;
	m_current_y = 0;
	m_immediate_exit = false;
	m_initial_x = 0;
	m_initial_y = 0;
	m_interaction_mode = NArraySupport::Passive;
	m_last_char = '0';
	m_visualisation_mode = NArraySupport::UseDataToSizeAndColour;
	m_window_scale_x = 0.0;
	m_window_scale_y = 0.0;
	m_window_scale_z = 0.0;
	m_window_size_x = 0;
	m_window_size_y = 0;
	m_window_start_x = 0;
	m_window_start_y = 0;
	m_zoom_x1 = 0.0;
	m_zoom_x2 = 0.0;
	m_zoom_y1 = 0.0;
	m_zoom_y2 = 0.0;
	m_zoom_z1 = 0.0;
	m_zoom_z2 = 0.0;
}

template <class S, class V> void Visualisation<S, V>::ColourMap(double val, double& point_colour_red, double& point_colour_green, double& point_colour_blue)
{
	double min = 0.0;
	double max = 1.0;
	double posSlope = (max - min) / 60;
	double negSlope = (min - max) / 60;

	if (val == 0.0)
	{
		point_colour_red = 0.0;
		point_colour_green = 0.0;
		point_colour_blue = 0.0;
	}
	else if (val < 60.0)
	{
		point_colour_red = max;
		point_colour_green = posSlope * val + min;
		point_colour_blue = min;
	}
	else if (val < 120.0)
	{
		point_colour_red = negSlope * val + 2 * max + min;
		point_colour_green = max;
		point_colour_blue = min;
	}
	else if (val < 180.0)
	{
		point_colour_red = min;
		point_colour_green = max;
		point_colour_blue = posSlope * val - 2 * max + min;
	}
	else if (val < 240.0)
	{
		point_colour_red = min;
		point_colour_green = negSlope * val + 4 * max + min;
		point_colour_blue = max;
	}
	else if (val < 300.0)
	{
		point_colour_red = posSlope * val - 4 * max + min;
		point_colour_green = min;
		point_colour_blue = max;
	}
	else
	{
		point_colour_red = max;
		point_colour_green = min;
		point_colour_blue = negSlope * val + 6 * max;
	}
}

template <class S, class V> void Visualisation<S, V>::DisplayCall()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int display_list = StartDisplayList + 1; display_list < EndDisplayList; display_list++)
		glCallList(display_list);
	glutSwapBuffers();
	if (Visualisation::GetTheInstance().m_immediate_exit)
	{
		Sleep(1000);
		glutLeaveMainLoop();
	}
}

template <class S, class V> void Visualisation<S, V>::DrawBox(DisplayList list, double start_x, double start_y, double start_z, double end_x, double end_y, double end_z)
{
	glNewList(list, GL_COMPILE);
	glPointSize(1.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINE_STRIP);
	glVertex3d(start_x,		start_y,	start_z);
	glVertex3d(end_x,		start_y,	start_z);
	glVertex3d(end_x,		end_y,		start_z);
	glVertex3d(start_x,		end_y,		start_z);
	glVertex3d(start_x,		start_y,	start_z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(start_x,		start_y,	end_z);
	glVertex3d(end_x,		start_y,	end_z);
	glVertex3d(end_x,		end_y,		end_z);
	glVertex3d(start_x,		end_y,		end_z);
	glVertex3d(start_x,		start_y,	end_z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(start_x,		start_y,	start_z);
	glVertex3d(start_x,		start_y,	end_z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(end_x,		start_y,	start_z);
	glVertex3d(end_x,		start_y,	end_z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(start_x,		end_y,		start_z);
	glVertex3d(start_x,		end_y,		end_z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(end_x,		end_y,		start_z);
	glVertex3d(end_x,		end_y,		end_z);
	glEnd();

	glEndList();
	glutPostRedisplay();
}

template <class S, class V> void Visualisation<S, V>::DrawText(DisplayList list, double start_x, double start_y, double start_z)
{
	glNewList(list, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos3d(start_x, start_y, start_z);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*) "Hello");
	glEndList();
	glutPostRedisplay();
}

template <class S, class V> Visualisation<S, V>& Visualisation<S, V>::GetTheInstance()
{
	static Visualisation the_instance;

	return the_instance;
}

template <class S, class V> void Visualisation<S, V>::GetZoom(double& x1, double& x2, double& y1, double& y2, double& z1, double& z2)
{
	x1 = GetTheInstance().m_zoom_x1;
	x2 = GetTheInstance().m_zoom_x2;
	y1 = GetTheInstance().m_zoom_y1;
	y2 = GetTheInstance().m_zoom_y2;
	z1 = GetTheInstance().m_zoom_z1;
	z2 = GetTheInstance().m_zoom_z2;
}

template <class S, class V> void Visualisation<S, V>::Initialise(NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	m_visualisation_mode = visualisation_mode;
	m_window_start_x = window_start_x;
	m_window_start_y = window_start_y;
	m_window_size_x = window_size_x;
	m_window_size_y = window_size_y;
	m_window_scale_x = (double) m_window_size_x / (double) m_window_resolution_x * 10.0;
	m_window_scale_y = (double) m_window_size_y / (double) m_window_resolution_y * 10.0;
	m_window_scale_z = 10.0;

	int argc = 1;
	char* argv = "Initialise";
	glutInit(&argc, &argv);
	glutInitWindowPosition(m_window_start_x, m_window_start_y);
	glutInitWindowSize(m_window_size_x, m_window_size_y);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("NArray Visualiser");

	glutDisplayFunc(DisplayCall);
	glutMotionFunc(MotionCall);
	glutKeyboardFunc(KeyboardCall);
	glutMouseFunc(MouseButtonCall);
	glutMouseWheelFunc(MouseWheelCall);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	double radius = 10.0;
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 2.0 * radius, 10.0 * radius);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 6.0 * radius, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble mvmatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	GLdouble projmatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	double start_x = -0.5 * m_window_scale_x;
	double start_y = -0.5 * m_window_scale_y;
	double start_z = -0.5 * m_window_scale_z;
	double end_x = 0.5 * m_window_scale_x;
	double end_y = 0.5 * m_window_scale_y;
	double end_z = 0.5 * m_window_scale_z;
	DrawBox(Frame, start_x, start_y, start_z, end_x, end_y, end_z);
}

template <class S, class V> void Visualisation<S, V>::KeyboardCall(unsigned char key, int x, int y)
{
	x; // do this to prevent compiler warning 'unreferenced formal parameter'
	y; // do this to prevent compiler warning 'unreferenced formal parameter'
	
	GetTheInstance().PutLastChar(key);

	if (key == '1' || key == '2' || key == '3' || key == '4' || key == 'n' || key == 'z' || key == 'r' || key == 'q')
		glutLeaveMainLoop();
}

template <class S, class V> void Visualisation<S, V>::MotionCall(int x, int y)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int xdelta = abs(GetTheInstance().m_current_x - x);
	int ydelta = abs(GetTheInstance().m_current_y - y);

	if ((xdelta < viewport[2] / 2 - 5 && ydelta < viewport[3] / 2 - 5) && (xdelta >= 1 || ydelta >= 1))
	{
		if (GetTheInstance().m_interaction_mode == NArraySupport::Rotate)
			GetTheInstance().RotateDisplay(x, y);
		else if (GetTheInstance().m_interaction_mode == NArraySupport::Zoom)
			GetTheInstance().ZoomDisplay(x, y);
	}

	GetTheInstance().m_current_x = x;
	GetTheInstance().m_current_y = y;
}

template <class S, class V> void Visualisation<S, V>::MouseButtonCall(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN && GetTheInstance().m_interaction_mode == NArraySupport::Passive)
		{
			GetTheInstance().m_interaction_mode = NArraySupport::Zoom;
			GetTheInstance().m_initial_x = x;
			GetTheInstance().m_initial_y = y;
		}
		else if (state == GLUT_UP && GetTheInstance().m_interaction_mode == NArraySupport::Zoom)
		{
			GetTheInstance().m_interaction_mode = NArraySupport::Passive;
			GetTheInstance().m_current_x = x;
			GetTheInstance().m_current_y = y;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN && GetTheInstance().m_interaction_mode == NArraySupport::Passive)
		{
			GetTheInstance().m_interaction_mode = NArraySupport::Rotate;
			GetTheInstance().m_current_x = x;
			GetTheInstance().m_current_y = y;
		}
		else if (state == GLUT_UP && GetTheInstance().m_interaction_mode == NArraySupport::Rotate)
		{
			GetTheInstance().m_interaction_mode = NArraySupport::Passive;
			GetTheInstance().m_current_x = x;
			GetTheInstance().m_current_y = y;
		}
	}
}

template <class S, class V> void Visualisation<S, V>::MouseWheelCall(int wheel, int direction, int x, int y)
{
	wheel; // do this to prevent compiler warning 'unreferenced formal parameter'
	x; // do this to prevent compiler warning 'unreferenced formal parameter'
	y; // do this to prevent compiler warning 'unreferenced formal parameter'

	double zoom = 1.0;
	if (direction == -1)
		zoom = 0.9;
	else if (direction == 1)
		zoom = 1.1;
	
	glMatrixMode(GL_MODELVIEW);
	glScaled(zoom, zoom, zoom);
	glutPostRedisplay();
}

template <class S, class V> void Visualisation<S, V>::ReadBitmapFile(std::fstream& input_file)
{
	struct CharInBits
	{
		bool bit1 : 1;
		bool bit2 : 1;
		bool bit3 : 1;
		bool bit4 : 1;
		bool bit5 : 1;
		bool bit6 : 1;
		bool bit7 : 1;
		bool bit8 : 1;
	};
	const int char_size = 1;
	const int short_size = 2;
	const int int_size = 4;
	char dummy_char[char_size];
	char dummy_short[short_size];
	char dummy_int[int_size];

	char header_field[3];
	input_file.read(dummy_char, char_size);
	header_field[0] = *dummy_char;
	input_file.read(dummy_char, char_size);
	header_field[1] = *dummy_char;
	header_field[2] = '\0';
	input_file.read(dummy_int, int_size);
	// int file_size = *((int*) &dummy_int);
	input_file.read(dummy_short, short_size);
	// short res1 = *((short*) &dummy_short);
	input_file.read(dummy_short, short_size);
	// short res2 = *((short*) &dummy_short);
	input_file.read(dummy_int, int_size);
	int offset = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int header_size = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	int bitmap_width = *(int*) &dummy_int;
	input_file.read(dummy_int, int_size);
	int bitmap_height = *((int*) &dummy_int);
	input_file.read(dummy_short, short_size);
	// short num_colour_planes = *((short*) &dummy_short);
	input_file.read(dummy_short, short_size);
	int num_bits_per_pixel = *((short*) &dummy_short);
	input_file.read(dummy_int, int_size);
	// int compression_method = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int image_size = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int horiz_res = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int vert_res = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int num_colours = *((int*) &dummy_int);
	input_file.read(dummy_int, int_size);
	// int num_imp_colours = *((int*) &dummy_int);
	input_file.seekg(offset, std::ios_base::beg);

	int num_cols = (int) floor((num_bits_per_pixel * abs(bitmap_width) + 31) / 32) * 4;
	int num_rows = abs(bitmap_height);
	
	Shape image_shape(c_matrix_order, num_rows, num_cols * 8);
	NArrayType<int>::Matrix image_in(NArraySupport::ContiguousInternal, image_shape, 0);
	NArrayType<int>::Matrix horiz_image_out(NArraySupport::ContiguousInternal, image_shape, 0);
	NArrayType<int>::Matrix vert_image_out(NArraySupport::ContiguousInternal, image_shape, 0);
	NArrayType<int>::Matrix image_out(NArraySupport::ContiguousInternal, image_shape, 0);
	
	Shape kernel_shape(c_matrix_order, 3, 3);
	NArrayType<int>::Matrix horiz_kernel(NArraySupport::ContiguousInternal, kernel_shape, 0);
	NArrayType<int>::Matrix vert_kernel(NArraySupport::ContiguousInternal, kernel_shape, 0);
	
	Mask kernel_mask(c_matrix_order);
	SetToValue<NArrayType<int>::Matrix, int> set_to_value;

	kernel_mask.ReMask(c_matrix_order, Mask::use_all, 1);
	set_to_value.Reset(-1);
	horiz_kernel.ApplySingleThreadedOperator(kernel_mask, set_to_value, horiz_kernel, horiz_kernel);
	kernel_mask.ReMask(c_matrix_order, Mask::use_all, 3);
	set_to_value.Reset(1);
	horiz_kernel.ApplySingleThreadedOperator(kernel_mask, set_to_value, horiz_kernel, horiz_kernel);

	kernel_mask.ReMask(c_matrix_order, 1, Mask::use_all);
	set_to_value.Reset(-1);
	vert_kernel.ApplySingleThreadedOperator(kernel_mask, set_to_value, vert_kernel, vert_kernel);
	kernel_mask.ReMask(c_matrix_order, 3, Mask::use_all);
	set_to_value.Reset(1);
	vert_kernel.ApplySingleThreadedOperator(kernel_mask, set_to_value, vert_kernel, vert_kernel);

	for (int row = 0; row < num_rows; row++)
	{
		for (int col = 0; col < num_cols; col++)
		{
			if (!input_file.good())
				break;

			input_file.read(dummy_char, char_size);
			CharInBits data = *((CharInBits*) &dummy_char);
			image_in[row + 1][col * 8 + 1][1] = data.bit8;
			image_in[row + 1][col * 8 + 2][1] = data.bit7;
			image_in[row + 1][col * 8 + 3][1] = data.bit6;
			image_in[row + 1][col * 8 + 4][1] = data.bit5;
			image_in[row + 1][col * 8 + 5][1] = data.bit4;
			image_in[row + 1][col * 8 + 6][1] = data.bit3;
			image_in[row + 1][col * 8 + 7][1] = data.bit2;
			image_in[row + 1][col * 8 + 8][1] = data.bit1;
		}
	}
	
	CalculateConvolution<NArrayType<int>::Matrix, int>(image_in, horiz_kernel, horiz_image_out);
	CalculateConvolution<NArrayType<int>::Matrix, int>(image_in, vert_kernel, vert_image_out);
	image_out = horiz_image_out + vert_image_out;
	
	Visualise2D<NArrayType<int>::Matrix, int>(image_in, NArraySupport::UseDataToColour);
	Visualise2D<NArrayType<int>::Matrix, int>(image_out, NArraySupport::UseDataToColour);
}

template <class S, class V> void Visualisation<S, V>::ResetDisplay()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

template <class S, class V> void Visualisation<S, V>::RotateDisplay(int x, int y)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble mvmatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	GLdouble projmatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	int tx1 = viewport[2] / 2;
	int ty1 = viewport[3] / 2;
	int ty1_flip = viewport[3] - ty1 - 1;
	double x1, y1, z1;
	gluUnProject(tx1, ty1_flip, 0.5, mvmatrix, projmatrix, viewport, &x1, &y1, &z1);

	int xdelta = GetTheInstance().m_current_x - x;
	int ydelta = GetTheInstance().m_current_y - y;
	int tx2 = tx1 + ydelta;
	int ty2 = ty1 - xdelta;
	int ty2_flip = viewport[3] - ty2 - 1;
	double x2, y2, z2;
	gluUnProject(tx2, ty2_flip, 0.5, mvmatrix, projmatrix, viewport, &x2, &y2, &z2);

	double rz = -180.0 * sqrt((double) (xdelta * xdelta) + (ydelta * ydelta)) / sqrt((double) (viewport[2] * viewport[2]) + (viewport[3] * viewport[3]));
	x1 = x2 - x1;
	y1 = y2 - y1;
	z1 = z2 - z1;

	glMatrixMode(GL_MODELVIEW);
	glRotated(rz, x1, y1, z1);
	glutPostRedisplay();
}

template <class S, class V> void Visualisation<S, V>::ZoomDisplay(int x, int y)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble mvmatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	GLdouble projmatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	double initial_z;
	double current_z;
	glReadPixels(GetTheInstance().m_initial_x, viewport[3] - GetTheInstance().m_initial_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &initial_z);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &current_z);

	double x1, y1, z1;
	gluUnProject(GetTheInstance().m_initial_x, viewport[3] - GetTheInstance().m_initial_y, initial_z, mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
	double x2, y2, z2;
	gluUnProject(x, viewport[3] - y, current_z, mvmatrix, projmatrix, viewport, &x2, &y2, &z2);

	DrawBox(Zoom, x1, y1, z1, x2, y2, z2);

	if (x1 > x2)
		std::swap(x1, x2);
	if (y1 > y2)
		std::swap(y1, y2);
	if (z1 > z2)
		std::swap(z1, z2);

	GetTheInstance().m_zoom_x1 = x1; // / m_window_scale_x;
	GetTheInstance().m_zoom_x2 = x2; // / m_window_scale_x;
	GetTheInstance().m_zoom_y1 = y1; // / m_window_scale_y;
	GetTheInstance().m_zoom_y2 = y2; // / m_window_scale_y;
	GetTheInstance().m_zoom_z1 = z1; // / m_window_scale_z;
	GetTheInstance().m_zoom_z2 = z2; // / m_window_scale_z;
}
