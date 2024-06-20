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

using namespace std;

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

void NArray::TestAllFunctions()
{
	bool cont;
	do
	{
		bool verbosity = false;
		bool valid_input = false;

		bool test_contiguous_internal;
		bool test_contiguous_external;
		bool test_distributed_internal;
		do
		{
			test_contiguous_internal = false;
			test_contiguous_external = false;
			test_distributed_internal = false;
			valid_input = false;

			cout << "Which memory model would you like to test?" << endl;
			cout << "1. Contiguous internal" << endl;
			cout << "2. Contiguous external (not all tests will test this memory model)" << endl;
			cout << "3. Distributed internal" << endl;
			cout << "4. All memory models" << endl;
			int user_input;
			cin >> user_input;

			switch (user_input)
			{
			case 1:
				test_contiguous_internal = true;
				valid_input = true;
				break;
			case 2:
				test_contiguous_external = true;
				valid_input = true;
				break;
			case 3:
				test_distributed_internal = true;
				valid_input = true;
				break;
			case 4:
				test_contiguous_internal = true;
				test_contiguous_external = true;
				test_distributed_internal = true;
				valid_input = true;
				break;
			default:
				cout << "Please enter a number between 1 and 4." << endl;
				valid_input = false;
				break;
			}
		} while (!valid_input);

		bool test_single_threaded;
		bool test_multi_threaded;
		double single_threaded_time = 0.0;
		double multi_threaded_time = 0.0;
		do
		{
			test_single_threaded = false;
			test_multi_threaded = false;
			valid_input = false;

			cout << "Which threading model would you like to test?" << endl;
			cout << "1. Single threaded" << endl;
			cout << "2. Multi threaded" << endl;
			cout << "3. Both threading models (allows comparisson of speeds)" << endl;
			int user_input;
			cin >> user_input;

			switch (user_input)
			{
			case 1:
				test_single_threaded = true;
				valid_input = true;
				break;
			case 2:
				test_multi_threaded = true;
				valid_input = true;
				break;
			case 3:
				test_single_threaded = true;
				test_multi_threaded = true;
				valid_input = true;
				break;
			default:
				cout << "Please enter a number between 1 and 3." << endl;
				valid_input = false;
				break;
			}
		} while (!valid_input);

		bool generate_reference_times;
		bool test_animation_functions;
		bool test_arithmetic_functions;
		bool test_combination_functions;
		bool test_communication_functions;
		bool test_comparison_functions;
		bool test_core;
		bool test_generic_type;
		bool test_multi_threading;
		bool test_serialisation;
		bool test_statistical_functions;
		bool test_useful_functions;
		bool test_visualisation_functions;
		do
		{
			generate_reference_times = false;
			test_animation_functions = false;
			test_arithmetic_functions = false;
			test_combination_functions = false;
			test_communication_functions = false;
			test_comparison_functions = false;
			test_core = false;
			test_generic_type = false;
			test_multi_threading = false;
			test_serialisation = false;
			test_statistical_functions = false;
			test_useful_functions = false;
			test_visualisation_functions = false;
			valid_input = false;

			cout << "What would you like to test?" << endl;
			cout << "1. Animation Functions" << endl;
			cout << "2. Arithmetic Functions" << endl;
			cout << "3. Combination Functions" << endl;
			cout << "4. Communication Functions" << endl;
			cout << "5. Comparison Functions" << endl;
			cout << "6. Core" << endl;
			cout << "7. Generic Type" << endl;
			cout << "8. Multi Threading" << endl;
			cout << "9. Serialisation" << endl;
			cout << "10. Statistical Functions" << endl;
			cout << "11. Useful Functions" << endl;
			cout << "12. Visualisation Functions" << endl;
			cout << "13. Everything!" << endl;
			int user_input;
			cin >> user_input;

			switch (user_input)
			{
			case 1:
				test_animation_functions = true;
				valid_input = true;
				break;
			case 2:
				test_arithmetic_functions = true;
				valid_input = true;
				break;
			case 3:
				test_combination_functions = true;
				valid_input = true;
				break;
			case 4:
				test_communication_functions = true;
				valid_input = true;
				break;
			case 5:
				test_comparison_functions = true;
				valid_input = true;
				break;
			case 6:
				test_core = true;
				valid_input = true;
				break;
			case 7:
				test_generic_type = true;
				valid_input = true;
				break;
			case 8:
				test_multi_threading = true;
				valid_input = true;
				break;
			case 9:
				test_serialisation = true;
				valid_input = true;
				break;
			case 10:
				test_statistical_functions = true;
				valid_input = true;
				break;
			case 11:
				test_useful_functions = true;
				valid_input = true;
				break;
			case 12:
				test_visualisation_functions = true;
				valid_input = true;
				break;
			case 13:
				generate_reference_times = true;
				test_animation_functions = true;
				test_arithmetic_functions = true;
				test_combination_functions = true;
				test_communication_functions = true;
				test_comparison_functions = true;
				test_core = true;
				test_generic_type = true;
				test_multi_threading = true;
				test_serialisation = true;
				test_statistical_functions = true;
				test_useful_functions = true;
				test_visualisation_functions = true;
				valid_input = true;
				break;
			default:
				cout << "Please enter a number between 1 and 13." << endl;
				valid_input = false;
				break;
			}
		} while (!valid_input);

		if (generate_reference_times)
		{
			string output_filename = "Testing/generate_reference_times_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			GenerateReferenceTimes(output_file);
		}

		if (test_animation_functions)
		{
			string output_filename = "Animation/test_animation_functions_out.txt";
			cout << "Output will be generated in " << output_filename << " and in the window that appears." << endl;
			cout << "In the window that appears, holding down the right mouse button and moving the mouse will rotate the image, press q to quit the current image and move to the next image." << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestAnimationFunctions(NArraySupport::ContiguousInternal, output_file);
			if (test_distributed_internal)
				TestAnimationFunctions(NArraySupport::DistributedInternal, output_file);
		}

		if (test_arithmetic_functions)
		{
			string output_filename = "Arithmetic/test_arithmetic_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
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
			string output_filename = "Combination/test_combination_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
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
			string output_filename = "Communication/test_communication_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestCommunicationFunctions(NArraySupport::ContiguousInternal, output_file);
			if (test_distributed_internal)
				TestCommunicationFunctions(NArraySupport::DistributedInternal, output_file);
		}

		if (test_comparison_functions)
		{
			string output_filename = "Comparison/test_comparison_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestComparisonFunctions(NArraySupport::ContiguousInternal, output_file);
			if (test_distributed_internal)
				TestComparisonFunctions(NArraySupport::DistributedInternal, output_file);
		}

		if (test_core)
		{
			string output_filename = "Core/test_core_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestCore(NArraySupport::ContiguousInternal, output_file, verbosity);
			if (test_contiguous_external)
				TestCore(NArraySupport::ContiguousExternal, output_file, verbosity);
			if (test_distributed_internal)
				TestCore(NArraySupport::DistributedInternal, output_file, verbosity);
		}

		if (test_generic_type)
		{
			string output_filename = "GenericType/test_generic_type_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestGenericType(NArraySupport::ContiguousInternal, output_file, verbosity);
			if (test_contiguous_external)
				TestGenericType(NArraySupport::ContiguousExternal, output_file, verbosity);
			if (test_distributed_internal)
				TestGenericType(NArraySupport::DistributedInternal, output_file, verbosity);
		}

		if (test_multi_threading)
		{
			string output_filename = "MultiThreading/test_multi_threading_out.txt";
			cout << "Output will be generated in " << output_filename << " and in the window that appears." << endl;
			cout << "In the window that appears, holding down the right mouse button and moving the mouse will rotate the image, press q to quit the current image and move to the next image." << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
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
			string output_filename = "Serialisation/test_serialisation_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestSerialisation(NArraySupport::ContiguousInternal, output_file, verbosity);
			if (test_distributed_internal)
				TestSerialisation(NArraySupport::DistributedInternal, output_file, verbosity);
		}

		if (test_statistical_functions)
		{
			string output_filename = "Statistical/test_statistical_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
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
			string output_filename = "Useful/test_useful_functions_out.txt";
			cout << "Output will be generated in " << output_filename << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
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
			string output_filename = "Visualisation/test_visualisation_functions_out.txt";
			cout << "Output will be generated in " << output_filename << " and in the window that appears." << endl;
			cout << "In the window that appears, holding down the right mouse button and moving the mouse will rotate the image, press q to quit the current image and move to the next image." << endl;
			std::fstream output_file(output_filename, std::ios_base::out);
			if (test_contiguous_internal)
				TestVisualisationFunctions(NArraySupport::ContiguousInternal, output_file);
			if (test_distributed_internal)
				TestVisualisationFunctions(NArraySupport::DistributedInternal, output_file);
		}

		cont = false;
		cout << "Do you wish to continue (y/n)?";
		char user_input;
		cin >> user_input;
		if (user_input == 'y')
		{
			cont = true;
		}
	} while (cont);
}
