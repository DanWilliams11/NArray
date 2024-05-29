#pragma once

#include "../MultiThreading/MultiThreading.h"
#include "StatisticalOperators.h"
#include "../Testing/Timer.h"

namespace NArray
{
	// function for finding the extrema (min and max) values in an n-dimensional array - tested
	template <class S, class V> void CalculateExtrema(S& in, V& min_value, Index& min_index, V& max_value, Index& max_index, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (timer) timer->StartTimer();
		Extrema<S, V> extrema;
		ApplySingleThreadedOperator(extrema, in);
		min_value = extrema.GetMinValue();
		min_index = extrema.GetMinIndex();
		max_value = extrema.GetMaxValue();
		max_index = extrema.GetMaxIndex();
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateExtrema' took " << timer->GetTime() << std::endl;
			*output_file << "Min Value: " << min_value << std::endl;
			*output_file << "Min Index: " << min_index << std::endl;
			*output_file << "Max Value: " << max_value << std::endl;
			*output_file << "Max Index: " << max_index << std::endl;
		}
	}

	// function for calculating the General Normal CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateGeneralNormalCDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the General Normal CDF of the values in <in> in function 'CalculateGeneralNormalCDF'\n"));

		if (timer) timer->StartTimer();
		GeneralNormalCDF<S, V> general_normal_cdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(general_normal_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(general_normal_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateGeneralNormalCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the General Normal Inverse CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateGeneralNormalInverseCDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the General Normal Inverse CDF of the values in <in> in function 'CalculateGeneralNormalInverseCDF'\n"));

		if (timer) timer->StartTimer();
		GeneralNormalInverseCDF<S, V> general_normal_inverse_cdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(general_normal_inverse_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(general_normal_inverse_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateGeneralNormalInverseCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the General Normal PDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateGeneralNormalPDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the General Normal PDF of the values in <in> in function 'CalculateGeneralNormalPDF'\n"));

		if (timer) timer->StartTimer();
		GeneralNormalPDF<S, V> general_normal_pdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(general_normal_pdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(general_normal_pdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateGeneralNormalPDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the Log Normal CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateLogNormalCDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Log Normal CDF of the values in <in> in function 'CalculateLogNormalCDF'\n"));

		if (timer) timer->StartTimer();
		LogNormalCDF<S, V> log_normal_cdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(log_normal_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(log_normal_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateLogNormalCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the Log Normal Inverse CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateLogNormalInverseCDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Log Normal Inverse CDF of the values in <in> in function 'CalculateLogNormalInverseCDF'\n"));

		if (timer) timer->StartTimer();
		LogNormalInverseCDF<S, V> log_normal_inverse_cdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(log_normal_inverse_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(log_normal_inverse_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateLogNormalInverseCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the Log Normal PDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateLogNormalPDF(S& in, V mu, V sigma, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Log Normal PDF of the values in <in> in function 'CalculateLogNormalPDF'\n"));

		if (timer) timer->StartTimer();
		LogNormalPDF<S, V> log_normal_pdf(mu, sigma);
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(log_normal_pdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(log_normal_pdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateLogNormalPDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the mean of the values in an n-dimensional array - tested
	template <class S, class V> V CalculateMean(S& in, Timer* timer = nullptr, std::fstream* output_file = nullptr)
	{
		if (timer) timer->StartTimer();
		Mean<S, V> mean;
		ApplySingleThreadedOperator(mean, in);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateMean' took " << timer->GetTime() << std::endl;
			*output_file << "Result: " << mean.GetMean() << std::endl;
		}
		return mean.GetMean();
	}

	// function for calculating the Standard Normal CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateStandardNormalCDF(S& in, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Standard Normal CDF of the values in <in> in function 'CalculateStandardNormalCDF'\n"));

		if (timer) timer->StartTimer();
		StandardNormalCDF<S, V> standard_normal_cdf;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(standard_normal_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(standard_normal_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateStandardNormalCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the Standard Normal Inverse CDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateStandardNormalInverseCDF(S& in, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Standard Normal Inverse CDF of the values in <in> in function 'CalculateStandardNormalInverseCDF'\n"));

		if (timer) timer->StartTimer();
		StandardNormalInverseCDF<S, V> standard_normal_inverse_cdf;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(standard_normal_inverse_cdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(standard_normal_inverse_cdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateStandardNormalInverseCDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}

	// function for calculating the Standard Normal PDF of the values in an n-dimensional array, writing the results to another n-dimensional array - tested
	template <class S, class V> void CalculateStandardNormalPDF(S& in, S& out, Mask* multi_threading_mask = nullptr, Timer* timer = nullptr, std::fstream* output_file = nullptr, bool verbosity = false)
	{
		if (in.GetShape() != out.GetShape())
			throw(std::invalid_argument("NArray: <in> and <out> n-dimensional arrays must be of the same shape if <out> is to be the Standard Normal PDF of the values in <in> in function 'CalculateStandardNormalPDF'\n"));

		if (timer) timer->StartTimer();
		StandardNormalPDF<S, V> standard_normal_pdf;
		if (multi_threading_mask)
			ApplyMultiThreadedOperator(standard_normal_pdf, in, out, multi_threading_mask);
		else
			ApplySingleThreadedOperator(standard_normal_pdf, in, out);
		if (timer) timer->StopTimer();
		if (timer && output_file && output_file->good())
		{
			*output_file << "Function 'CalculateStandardNormalPDF' took " << timer->GetTime() << std::endl;
			out.PrettyPrint(*output_file, verbosity);
		}
	}
}
