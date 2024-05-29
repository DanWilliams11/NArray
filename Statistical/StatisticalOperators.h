#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include "../Arithmetic/ArithmeticOperators.h"

namespace NArray
{
	// class for finding the extrema (min and max) values in an n-dimensional array - tested
	// this operator should not be multi-threaded because it is stateful
	// access to the state would need to be mutexed and this would result in poor performance (thread-contention)
	template <class S, class V> class Extrema : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		bool m_first_call;
		Index m_max_index;
		V m_max_value;
		Index m_min_index;
		V m_min_value;
		// member functions
	public:
		Extrema()
		{
			Reset();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			if (m_first_call || value < m_min_value)
			{
				m_min_index = index;
				m_min_value = value;
			}
			if (m_first_call || value > m_max_value)
			{
				m_max_index = index;
				m_max_value = value;
			}
			m_first_call = false;
			return true;
		}
		Index GetMaxIndex() const
		{
			return m_max_index;
		}
		V GetMaxValue() const
		{
			return m_max_value;
		}
		Index GetMinIndex() const
		{
			return m_min_index;
		}
		V GetMinValue() const
		{
			return m_min_value;
		}
		void Reset()
		{
			m_first_call = true;
		}
	};

	// class for calculating the mean of the values in an n-dimensional array - tested
	template <class S, class V> class Mean : public Sigma<S, V>
	{
		// member variables
	private:
		int m_count;
		// member functions
	public:
		Mean()
		{
			Reset();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			Sigma::ApplyOperator(index, value, in, out);
			m_count++;
			return true;
		}
		V GetMean() const
		{
			return GetSigma() / m_count;
		}
		void Reset()
		{
			Sigma::Reset();
			m_count = 0;
		}
	};

	// abstract base class for statistical operators - tested
	template <class S, class V> class StatisticalOperator abstract : public MultiThreadedOperator<S, V>
	{
		// member variables
	protected:
		static const double m_root_two_pi;
	};

	template <class S, class V> const double StatisticalOperator<S, V>::m_root_two_pi = pow(2.0 * M_PI, 0.5);

	// abstract base class for mapping the values in an n-dimensional array to their CDF values - tested
	template <class S, class V> class CDF abstract : public StatisticalOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V p = CalcCDF(value);
			out.PutValue(index, p);
			return true;
		}
		virtual V CalcCDF(V x) = 0;
	};

	// abstract base class for mapping the values in an n-dimensional array to their Inverse CDF values - tested
	template <class S, class V> class InverseCDF abstract : public StatisticalOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V x = CalcInverseCDF(value);
			out.PutValue(index, x);
			return true;
		}
		virtual V CalcInverseCDF(V p) = 0;
	};

	// abstract base class for mapping the values in an n-dimensional array to their PDF values - tested
	template <class S, class V> class PDF abstract : public StatisticalOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V p = CalcPDF(value);
			out.PutValue(index, p);
			return true;
		}
		virtual V CalcPDF(V x) = 0;
	};

	// class for mapping the values in an n-dimensional array to their Standard Normal CDF values - tested
	template <class S, class V> class StandardNormalCDF : public CDF<S, V>
	{
		// member functions
	public:
		virtual V CalcCDF(V x)
		{
			V sum = x;
			V value = x;
			for (int i = 1; i < 100; i++)
			{
				value = value * x * x / (2 * i + 1);
				sum += value;
			}
			return 0.5 + (sum / m_root_two_pi) * exp(-(x * x) / 2.0);
		}
	};

	// class for mapping the values in an n-dimensional array to their Standard Normal Inverse CDF values - tested
	template <class S, class V> class StandardNormalInverseCDF : public InverseCDF<S, V>
	{
		// member functions
	public:
		virtual V CalcInverseCDF(V p)
		{
			double a[6];
			a[0] = -3.969683028665376e+01;
			a[1] = 2.209460984245205e+02;
			a[2] = -2.759285104469687e+02;
			a[3] = 1.383577518672690e+02;
			a[4] = -3.066479806614716e+01;
			a[5] = 2.506628277459239e+00;

			double b[5];
			b[0] = -5.447609879822406e+01;
			b[1] = 1.615858368580409e+02;
			b[2] = -1.556989798598866e+02;
			b[3] = 6.680131188771972e+01;
			b[4] = -1.328068155288572e+01;

			double c[6];
			c[0] = -7.784894002430293e-03;
			c[1] = -3.223964580411365e-01;
			c[2] = -2.400758277161838e+00;
			c[3] = -2.549732539343734e+00;
			c[4] = 4.374664141464968e+00;
			c[5] = 2.938163982698783e+00;

			double d[4];
			d[0] = 7.784695709041462e-03;
			d[1] = 3.224671290700398e-01;
			d[2] = 2.445134137142996e+00;
			d[3] = 3.754408661907416e+00;

			double p_low = 0.02425;
			double p_high = 1 - p_low;
			double q = 0.0;
			double r = 0.0;
			double x = 0.0;

			if (0.0 < p  && p < p_low)
			{
				q = pow(-2.0 * log(p), 0.5);
				x = (((((c[0] * q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5]) / ((((d[0] * q + d[1])*q + d[2])*q + d[3])*q + 1.0);
			}
			else if (p_low <= p && p <= p_high)
			{
				q = p - 0.5;
				r = q*q;
				x = (((((a[0] * r + a[1])*r + a[2])*r + a[3])*r + a[4])*r + a[5])*q / (((((b[0] * r + b[1])*r + b[2])*r + b[3])*r + b[4])*r + 1.0);
			}
			else if (p_high < p && p < 1.0)
			{
				q = pow(-2.0 * log(1.0 - p), 0.5);
				x = (((((c[0] * q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5]) / ((((d[0] * q + d[1])*q + d[2])*q + d[3])*q + 1.0);
			}

			return x;
		}
	};

	// class for mapping the values in an n-dimensional array to their Standard Normal PDF values - tested
	template <class S, class V> class StandardNormalPDF : public PDF<S, V>
	{
		// member functions
	public:
		virtual V CalcPDF(V x)
		{
			return exp(-0.5 * x * x) / m_root_two_pi;
		}
	};

	// class for mapping the values in an n-dimensional array to their General Normal CDF values - tested
	template <class S, class V> class GeneralNormalCDF : public StandardNormalCDF<S, V>
	{
		// member variables
	protected:
		V m_mu;
		V m_sigma;
		// member functions
	public:
		GeneralNormalCDF(V mu, V sigma)
			: m_mu(mu), m_sigma(sigma)
		{
		}
		virtual V CalcCDF(V x)
		{
			return (1.0 / m_sigma) * StandardNormalCDF::CalcCDF((x - m_mu) / m_sigma);
		}
	};

	// class for mapping the values in an n-dimensional array to their General Normal Inverse CDF values - tested
	template <class S, class V> class GeneralNormalInverseCDF : public StandardNormalInverseCDF<S, V>
	{
		// member variables
	protected:
		V m_mu;
		V m_sigma;
		// member functions
	public:
		GeneralNormalInverseCDF(V mu, V sigma)
			: m_mu(mu), m_sigma(sigma)
		{
		}
		virtual V CalcInverseCDF(V p)
		{
			return StandardNormalInverseCDF::CalcInverseCDF(p); // TODO correct this for mu and sigma
		}
	};

	// class for mapping the values in an n-dimensional array to their General Normal PDF values - tested
	template <class S, class V> class GeneralNormalPDF : public StandardNormalPDF<S, V>
	{
		// member variables
	protected:
		V m_mu;
		V m_sigma;
		// member functions
	public:
		GeneralNormalPDF(V mu, V sigma)
			: m_mu(mu), m_sigma(sigma)
		{
		}
		virtual V CalcPDF(V x)
		{
			return (1.0 / m_sigma) * StandardNormalPDF::CalcPDF((x - m_mu) / m_sigma);
		}
	};

	// class for mapping the values in an n-dimensional array to their Log Normal CDF values - tested
	template <class S, class V> class LogNormalCDF : public GeneralNormalCDF<S, V>
	{
		// member functions
	public:
		LogNormalCDF(V mu, V sigma)
			: GeneralNormalCDF(mu, sigma)
		{
		}
		virtual V CalcCDF(V x)
		{
			if (x > 0.0)
				return StandardNormalCDF::CalcCDF((log(x) - m_mu) / m_sigma);
			else
				return 0.0;
		}
	};

	// class for mapping the values in an n-dimensional array to their Log Normal Inverse CDF values - tested
	template <class S, class V> class LogNormalInverseCDF : public GeneralNormalInverseCDF<S, V>
	{
		// member functions
	public:
		LogNormalInverseCDF(V mu, V sigma)
			: GeneralNormalInverseCDF(mu, sigma)
		{
		}
		virtual V CalcInverseCDF(V p)
		{
			return exp(m_sigma * StandardNormalInverseCDF::CalcInverseCDF(p));
		}
	};

	// class for mapping the values in an n-dimensional array to their Log Normal PDF values - tested
	template <class S, class V> class LogNormalPDF : public GeneralNormalPDF<S, V>
	{
		// member variables
	private:
		double m_sigma_root_two_pi;
		double m_two_sigma_squared;
		// member functions
	public:
		LogNormalPDF(V mu, V sigma)
			: GeneralNormalPDF(mu, sigma)
		{
			m_sigma_root_two_pi = m_sigma * m_root_two_pi;
			m_two_sigma_squared = 2.0 * pow(m_sigma, 2);
		}
		virtual V CalcPDF(V x)
		{
			if (x > 0.0)
				return pow(x * m_sigma_root_two_pi, -1) * exp(-(pow(log(x) - m_mu, 2.0) / m_two_sigma_squared));
			else
				return 0.0;
		}
	};
}
