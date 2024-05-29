#pragma once

#include "../Visualisation/VisualisationDefinition.h"

namespace NArray
{
	// singleton class for animation using the OpenGL freeglut routines
	// singleton because there can only ever be one instance of the OpenGL environment
	// the OpenGL environment must run in the main thread and therefore cannot be multi-threaded
	template <class S, class V> class Animation : public Visualisation<S, V>
	{
		// member data
	protected:
		S* m_animations;

		// member functions
	protected:
		Animation();
		void Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
	};

	template <class S, class V> class Animation0D : public Animation<S, V>
	{
		// member functions
	private:
		static	void IdleCall();
	public:
		static	Animation0D& GetTheInstance();
		void Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
	};
	
	template <class S, class V> class Animation1D : public Animation<S, V>
	{
		// member functions
	private:
		static	void IdleCall();
	public:
		static	Animation1D& GetTheInstance();
		void Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
	};
	
	template <class S, class V> class Animation2D : public Animation<S, V>
	{
		// member functions
	private:
		static	void IdleCall();
	public:
		static	Animation2D& GetTheInstance();
		void Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
	};
	
	template <class S, class V> class Animation3D : public Animation<S, V>
	{
		// member functions
	private:
		static	void IdleCall();
	public:
		static	Animation3D& GetTheInstance();
		void Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y);
	};
}
