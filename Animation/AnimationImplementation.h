#pragma once

#include "GL/freeglut.h"

#include "AnimationDefinition.h"

using namespace NArray;

template <class S, class V> Animation<S, V>::Animation()
{
	m_animations = nullptr;
}

template <class S, class V> void Animation<S, V>::Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	m_animations = animations;
	Initialise(visualisation_mode, window_start_x, window_start_y, window_size_x, window_size_y);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

template <class S, class V> Animation0D<S, V>& Animation0D<S, V>::GetTheInstance()
{
	static Animation0D the_instance;

	return the_instance;
}

template <class S, class V> void Animation0D<S, V>::IdleCall()
{
	if (GetTheInstance().m_animations)
	{
		glNewList(Data, GL_COMPILE);
		Animate0D<S, V>(*GetTheInstance().m_animations, GetTheInstance().m_visualisation_mode);
		glEndList();
		glutPostRedisplay();
	}
}

template <class S, class V> void Animation0D<S, V>::Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	Animation<S, V>::Animate(animations, visualisation_mode, window_start_x, window_start_y, window_size_x, window_size_y);
	glutIdleFunc(IdleCall);
	glutMainLoop();
}

template <class S, class V> Animation1D<S, V>& Animation1D<S, V>::GetTheInstance()
{
	static Animation1D the_instance;

	return the_instance;
}

template <class S, class V> void Animation1D<S, V>::IdleCall()
{
	if (GetTheInstance().m_animations)
	{
		glNewList(Data, GL_COMPILE);
		Animate1D<S, V>(*GetTheInstance().m_animations, GetTheInstance().m_visualisation_mode);
		glEndList();
		glutPostRedisplay();
	}
}

template <class S, class V> void Animation1D<S, V>::Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	Animation<S, V>::Animate(animations, visualisation_mode, window_start_x, window_start_y, window_size_x, window_size_y);
	glutIdleFunc(IdleCall);
	glutMainLoop();
}

template <class S, class V> Animation2D<S, V>& Animation2D<S, V>::GetTheInstance()
{
	static Animation2D the_instance;

	return the_instance;
}

template <class S, class V> void Animation2D<S, V>::IdleCall()
{
	if (GetTheInstance().m_animations)
	{
		glNewList(Data, GL_COMPILE);
		Animate2D<S, V>(*GetTheInstance().m_animations, GetTheInstance().m_visualisation_mode);
		glEndList();
		glutPostRedisplay();
	}
}

template <class S, class V> void Animation2D<S, V>::Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	Animation<S, V>::Animate(animations, visualisation_mode, window_start_x, window_start_y, window_size_x, window_size_y);
	glutIdleFunc(IdleCall);
	glutMainLoop();
}

template <class S, class V> Animation3D<S, V>& Animation3D<S, V>::GetTheInstance()
{
	static Animation3D the_instance;

	return the_instance;
}

template <class S, class V> void Animation3D<S, V>::IdleCall()
{
	if (GetTheInstance().m_animations)
	{
		glNewList(Data, GL_COMPILE);
		Animate3D<S, V>(*GetTheInstance().m_animations, GetTheInstance().m_visualisation_mode);
		glEndList();
		glutPostRedisplay();
	}
}

template <class S, class V> void Animation3D<S, V>::Animate(S* animations, NArraySupport::VisualisationMode visualisation_mode, int window_start_x, int window_start_y, int window_size_x, int window_size_y)
{
	Animation<S, V>::Animate(animations, visualisation_mode, window_start_x, window_start_y, window_size_x, window_size_y);
	glutIdleFunc(IdleCall);
	glutMainLoop();
}
