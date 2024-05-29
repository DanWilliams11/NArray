#pragma once

#include "../Core/NArrayDefinition.h"

namespace NArray
{
	// singleton class for visualisation using the OpenGL freeglut routines
	// singleton because there can only ever be one instance of the OpenGL environment
	// the OpenGL environment must run in the main thread and therefore cannot be multi-threaded
	template <class S, class V> class Visualisation
	{
		// member data
	public:
		enum DisplayList
		{
			StartDisplayList,
			Data,
			Text,
			Frame,
			Zoom,
			EndDisplayList,
		};
	protected:
		int m_current_x;
		int m_current_y;
		bool m_immediate_exit;
		int m_initial_x;
		int m_initial_y;
		NArraySupport::InteractionMode m_interaction_mode;
		char m_last_char;
		NArraySupport::VisualisationMode m_visualisation_mode;
		static const int m_window_resolution_x = 1920; // TODO ask the system for this information
		static const int m_window_resolution_y = 1080; // TODO ask the system for this information
		double m_window_scale_x;
		double m_window_scale_y;
		double m_window_scale_z;
		int m_window_size_x;
		int m_window_size_y;
		int m_window_start_x;
		int m_window_start_y;
		double m_zoom_x1;
		double m_zoom_x2;
		double m_zoom_y1;
		double m_zoom_y2;
		double m_zoom_z1;
		double m_zoom_z2;

		// member functions
	protected:
				Visualisation();
	public:
		static	void ColourMap(double val, double& point_colour_red, double& point_colour_green, double& point_colour_blue);
		static	void DisplayCall();
		static	void DrawBox(DisplayList list, double start_x, double start_y, double start_z, double end_x, double end_y, double end_z);
		static	void DrawText(DisplayList list, double start_x, double start_y, double start_z);
		static	char GetLastChar() { return GetTheInstance().m_last_char; };
		static	Visualisation& GetTheInstance();
		static	void GetZoom(double& x1, double& x2, double& y1, double& y2, double& z1, double& z2);
		virtual	void Initialise(NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
		static	void KeyboardCall(unsigned char key, int x, int y);
		static	void MotionCall(int x, int y);
		static	void MouseButtonCall(int button, int state, int x, int y);
		static	void MouseWheelCall(int wheel, int direction, int x, int y);
		static	void PutLastChar(char last_char) { GetTheInstance().m_last_char = last_char; };
		static	void ReadBitmapFile(std::fstream& input_file);
		static	void ResetDisplay();
		static	void RotateDisplay(int x, int y);
				void ZoomDisplay(int x, int y);
	};
}
