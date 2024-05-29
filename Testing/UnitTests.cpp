#include <fstream>
#include <vector>

#include "../Animation/TestAnimationFunctions.h"
#include "../Arithmetic/TestArithmeticFunctions.h"
#include "../Combination/TestCombinationFunctions.h"
#include "../Communication/TestCommunicationFunctions.h"
#include "../Comparison/TestComparisonFunctions.h"
#include "../Core/NArrayImplementation.h"
#include "../Core/TestCore.h"
#include "../GenericType/TestGenericType.h"
#include "../MultiThreading/TestMultiThreading.h"
#include "../Serialisation/TestSerialisation.h"
#include "../Statistical/TestStatisticalFunctions.h"
#include "../Useful/TestUsefulFunctions.h"
#include "../Visualisation/TestVisualisationFunctions.h"
#include "Timer.h"
#include "UnitTests.h"

// functions

void NArray::CompareTimes(NArraySupport::StorageOption storage_option, std::fstream& output_file, double single_threaded_time, double multi_threaded_time)
{
	output_file << "For " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	double ratio = 0.0;
	if (single_threaded_time < multi_threaded_time)
	{
		ratio = multi_threaded_time / single_threaded_time;
		output_file << NArraySupport::MultiThreadingStr(false) << " was " << ratio << " times faster than " << NArraySupport::MultiThreadingStr(true) << std::endl << std::endl;
	}
	else
	{
		ratio = single_threaded_time / multi_threaded_time;
		output_file << NArraySupport::MultiThreadingStr(true) << " was " << ratio << " times faster than " << NArraySupport::MultiThreadingStr(false) << std::endl << std::endl;
	}
}

void NArray::GenerateReferenceTimes(std::fstream& output_file)
{
	output_file << "*** Reference Times - for comparing NArray-style array times with C-style array times and C++-style array times" << std::endl << std::endl;

	Timer timer;
	int num_rows = 1000;
	int num_cols = 1000;

	timer.StartTimer();
	{
		double** matrix = new double*[num_rows];
		for (int i = 0; i < num_rows; i++)
		{
			matrix[i] = new double[num_cols];
			for (int j = 0; j < num_cols; j++)
			{
				matrix[i][j] = 0.0;
			}
		}
		for (int i = 0; i < num_rows; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
	}
	timer.StopTimer();
	output_file << "Building and destroying the C-style array took " << timer.GetTime() << std::endl << std::endl;

	timer.StartTimer();
	{
		std::vector<std::vector<double>> matrix(num_rows);
		for (int i = 0; i < num_rows; i++)
		{
			std::vector<double> vector(num_cols);
			for (int j = 0; j < num_cols; j++)
			{
				vector[j] = 0.0;
			}
			matrix[i] = vector;
		}
	}
	timer.StopTimer();
	output_file << "Building and destroying the C++-style array took " << timer.GetTime() << std::endl << std::endl;

	for (int i = 0; i < 2; i++)
	{
		bool reindex = false;
		if (i == 1)
			reindex = true;

		timer.StartTimer();
		{
			Shape shape(c_matrix_order, num_rows, num_cols);
			NArrayType<double>::Matrix matrix(NArraySupport::ContiguousInternal, shape, 0.0, reindex);
		}
		timer.StopTimer();
		output_file << "Building and destroying the " << NArraySupport::StorageOptionStr(NArraySupport::ContiguousInternal) << " allocated NArray-style array with indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl << std::endl;

		timer.StartTimer();
		{
			Shape shape(c_matrix_order, num_rows, num_cols);
			double* arr = new double[num_rows * num_cols];
			NArrayType<double>::Matrix matrix(NArraySupport::ContiguousExternal, shape, arr, reindex);
			delete[] arr;
		}
		timer.StopTimer();
		output_file << "Building and destroying the " << NArraySupport::StorageOptionStr(NArraySupport::ContiguousExternal) << " allocated NArray-style array with indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl << std::endl;

		timer.StartTimer();
		{
			Shape shape(c_matrix_order, num_rows, num_cols);
			NArrayType<double>::Matrix matrix(NArraySupport::DistributedInternal, shape, 0.0, reindex);
		}
		timer.StopTimer();
		output_file << "Building and destroying the " << NArraySupport::StorageOptionStr(NArraySupport::DistributedInternal) << " allocated NArray-style array with indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl << std::endl;
	}
}

void NArray::TestAllFunctions(bool verbosity)
{
	bool all = true;
	bool test_contiguous_internal = all;
	bool test_contiguous_external = all;
	bool test_distributed_internal = all;

	bool test_single_threaded = all;
	bool test_multi_threaded = all;
	double single_threaded_time = 0.0;
	double multi_threaded_time = 0.0;

	all = true;
	bool generate_reference_times = all;
	bool test_animation_functions = all;
	bool test_arithmetic_functions = all;
	bool test_combination_functions = all;
	bool test_communication_functions = all;
	bool test_comparison_functions = all;
	bool test_core = all;
	bool test_generic_type = all;
	bool test_multi_threading = all;
	bool test_serialisation = all;
	bool test_statistical_functions = all;
	bool test_useful_functions = all;
	bool test_visualisation_functions = all;
	
	if (generate_reference_times)
	{
		std::fstream output_file("Testing/generate_reference_times_out.txt", std::ios_base::out);
		GenerateReferenceTimes(output_file);
	}

	if (test_animation_functions)
	{
		std::fstream output_file("Animation/test_animation_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestAnimationFunctions(NArraySupport::ContiguousInternal, output_file);
		if (test_distributed_internal)
			TestAnimationFunctions(NArraySupport::DistributedInternal, output_file);
	}

	if (test_arithmetic_functions)
	{
		std::fstream output_file("Arithmetic/test_arithmetic_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestArithmeticFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestArithmeticFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::ContiguousInternal, output_file, single_threaded_time, multi_threaded_time);
		}
		if (test_distributed_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestArithmeticFunctions(NArraySupport::DistributedInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestArithmeticFunctions(NArraySupport::DistributedInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::DistributedInternal, output_file, single_threaded_time, multi_threaded_time);
		}
	}

	if (test_combination_functions)
	{
		std::fstream output_file("Combination/test_combination_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestCombinationFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestCombinationFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::ContiguousInternal, output_file, single_threaded_time, multi_threaded_time);
		}
		if (test_distributed_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestCombinationFunctions(NArraySupport::DistributedInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestCombinationFunctions(NArraySupport::DistributedInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::DistributedInternal, output_file, single_threaded_time, multi_threaded_time);
		}
	}

	if (test_communication_functions)
	{
		std::fstream output_file("Communication/test_communication_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestCommunicationFunctions(NArraySupport::ContiguousInternal, output_file);
		if (test_distributed_internal)
			TestCommunicationFunctions(NArraySupport::DistributedInternal, output_file);
	}

	if (test_comparison_functions)
	{
		std::fstream output_file("Comparison/test_comparison_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestComparisonFunctions(NArraySupport::ContiguousInternal, output_file);
		if (test_distributed_internal)
			TestComparisonFunctions(NArraySupport::DistributedInternal, output_file);
	}

	if (test_core)
	{
		std::fstream output_file("Core/test_core_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestCore(NArraySupport::ContiguousInternal, output_file, verbosity);
		if (test_contiguous_external)
			TestCore(NArraySupport::ContiguousExternal, output_file, verbosity);
		if (test_distributed_internal)
			TestCore(NArraySupport::DistributedInternal, output_file, verbosity);
	}

	if (test_generic_type)
	{
		std::fstream output_file("GenericType/test_generic_type_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestGenericType(NArraySupport::ContiguousInternal, output_file, verbosity);
		if (test_contiguous_external)
			TestGenericType(NArraySupport::ContiguousExternal, output_file, verbosity);
		if (test_distributed_internal)
			TestGenericType(NArraySupport::DistributedInternal, output_file, verbosity);
	}

	if (test_multi_threading)
	{
		std::fstream output_file("MultiThreading/test_multi_threading_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestMultiThreading(NArraySupport::ContiguousInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestMultiThreading(NArraySupport::ContiguousInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::ContiguousInternal, output_file, single_threaded_time, multi_threaded_time);
		}
		if (test_distributed_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestMultiThreading(NArraySupport::DistributedInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestMultiThreading(NArraySupport::DistributedInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::DistributedInternal, output_file, single_threaded_time, multi_threaded_time);
		}
	}

	if (test_serialisation)
	{
		std::fstream output_file("Serialisation/test_serialisation_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestSerialisation(NArraySupport::ContiguousInternal, output_file, verbosity);
		if (test_distributed_internal)
			TestSerialisation(NArraySupport::DistributedInternal, output_file, verbosity);
	}

	if (test_statistical_functions)
	{
		std::fstream output_file("Statistical/test_statistical_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestStatisticalFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestStatisticalFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::ContiguousInternal, output_file, single_threaded_time, multi_threaded_time);
		}
		if (test_distributed_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestStatisticalFunctions(NArraySupport::DistributedInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestStatisticalFunctions(NArraySupport::DistributedInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::DistributedInternal, output_file, single_threaded_time, multi_threaded_time);
		}
	}

	if (test_useful_functions)
	{
		std::fstream output_file("Useful/test_useful_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestUsefulFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestUsefulFunctions(NArraySupport::ContiguousInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::ContiguousInternal, output_file, single_threaded_time, multi_threaded_time);
		}
		if (test_distributed_internal)
		{
			if (test_single_threaded)
				single_threaded_time = TestUsefulFunctions(NArraySupport::DistributedInternal, output_file, verbosity, false);
			if (test_multi_threaded)
				multi_threaded_time = TestUsefulFunctions(NArraySupport::DistributedInternal, output_file, verbosity, true);
			if (test_single_threaded && test_multi_threaded)
				CompareTimes(NArraySupport::DistributedInternal, output_file, single_threaded_time, multi_threaded_time);
		}
	}

	if (test_visualisation_functions)
	{
		std::fstream output_file("Visualisation/test_visualisation_functions_out.txt", std::ios_base::out);
		if (test_contiguous_internal)
			TestVisualisationFunctions(NArraySupport::ContiguousInternal, output_file);
		if (test_distributed_internal)
			TestVisualisationFunctions(NArraySupport::DistributedInternal, output_file);
	}
}
