#pragma once

#include "../Core/Transform.h"
#include "../VisualisationExamples/JuliaSet.h"
#include "../VisualisationExamples/MandelbrotSet.h"

namespace NArray
{
	// class for transforming an n-dimensional array of complex numbers to an n-dimensional array of their arguments - tested
	template <class S1, class V1, class S2, class V2> class Argument : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V2 argument = atan(value.imag() / value.real());
			out.PutValue(index, argument);
			return true;
		}
	};

	// class for transforming a matrix of complex numbers to a matrix of their Julia Set - tested
	template <class S1, class V1, class S2, class V2> class JuliaSet : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member variables
	private:
		V1 m_seed;
		
		// member functions
	private:
		JuliaSet(const JuliaSet&);
		JuliaSet& operator =(const JuliaSet&);
	public:
		JuliaSet(V1 seed)
			: m_seed(seed)
		{
			// all initialisation done in intialiser list
		}
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			ChaoticDynamicalSystems::JuliaSet<V1> julia_set(m_seed, value);
			out.PutValue(index, julia_set.GetData());
			return true;
		}
	};

	// class for transforming a matrix of complex numbers to a matrix of their Mandelbrot Set - tested
	template <class S1, class V1, class S2, class V2> class MandelbrotSet : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	private:
		MandelbrotSet(const MandelbrotSet&);
		MandelbrotSet& operator =(const MandelbrotSet&);
	public:
		MandelbrotSet()
		{
			// needed by compiler
		}
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			ChaoticDynamicalSystems::MandelbrotSet<V1> mandelbrot_set(value);
			out.PutValue(index, mandelbrot_set.GetData());
			return true;
		}
	};

	// class for transforming an n-dimensional array of complex numbers to an n-dimensional array of their moduli - tested
	template <class S1, class V1, class S2, class V2> class Modulus : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V2 modulus = sqrt(pow(value.real(), 2) + pow(value.imag(), 2));
			out.PutValue(index, modulus);
			return true;
		}
	};
}
