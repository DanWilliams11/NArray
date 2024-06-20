#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Testing/Timer.h"
#include "../Useful/UsefulFunctions.h"
#include "StatisticalFunctions.h"
#include "TestStatisticalFunctions.h"

double NArray::TestStatisticalFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool multi_threaded)
{
	std::cout << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::cout << "Statistical Functions" << std::endl << std::endl;
	output_file << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Statistical Functions" << std::endl << std::endl;

	Mask multi_threading_mask(c_matrix_order, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::not_use);
	Mask* multi_threading_mask_ptr = nullptr;
	if (multi_threaded)
		multi_threading_mask_ptr = &multi_threading_mask;

	Timer timer;
	int num_rows = 1000;
	int num_cols = 1000;
	Shape shape(c_matrix_order, num_rows, num_cols + 1);

	typedef double stored_type;
	typedef NArrayType<stored_type>::Matrix storage_type;
	storage_type output_matrix(storage_option, shape, stored_type(0.0), true);

	stored_type start_val = -3.0;
	stored_type end_val = 3.0;
	stored_type step_val = (end_val - start_val) / shape.GetTotalSize();
	storage_type input_matrix1(storage_option, shape, stored_type(0.0), true);
	MakeRamped<storage_type, stored_type>(input_matrix1, start_val, step_val, &timer, &output_file, verbosity);

	start_val = 0.0;
	end_val = 1.0;
	step_val = (end_val - start_val) / shape.GetTotalSize();
	storage_type input_matrix2(storage_option, shape, stored_type(0.0), true);
	MakeRamped<storage_type, stored_type>(input_matrix2, start_val, step_val, &timer, &output_file, verbosity);

	std::cout << "Test CalculateExtrema and CalculateMean" << std::endl;
	stored_type min_value = 0.0;
	stored_type max_value = 0.0;
	Index min_index;
	Index max_index;
	CalculateExtrema<storage_type, stored_type>(input_matrix1, min_value, min_index, max_value, max_index, &timer, &output_file);
	CalculateExtrema<storage_type, stored_type>(input_matrix2, min_value, min_index, max_value, max_index, &timer, &output_file);
	CalculateMean<storage_type, stored_type>(input_matrix1, &timer, &output_file);
	CalculateMean<storage_type, stored_type>(input_matrix2, &timer, &output_file);

	std::cout << "Test CalculateStandardNormalPDF, CalculateStandardNormalCDF and CalculateStandardNormalInverseCDF" << std::endl;
	stored_type mu = 0.0;
	stored_type sigma = 1.0;
	CalculateStandardNormalPDF<storage_type, stored_type>(input_matrix1, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateStandardNormalCDF<storage_type, stored_type>(input_matrix1, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateStandardNormalInverseCDF<storage_type, stored_type>(input_matrix2, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	std::cout << "Test CalculateGeneralNormalPDF, CalculateGeneralNormalCDF and CalculateGeneralNormalInverseCDF" << std::endl;
	mu = 2.0;
	sigma = 0.25;
	CalculateGeneralNormalPDF(input_matrix1, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateGeneralNormalCDF(input_matrix1, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateGeneralNormalInverseCDF(input_matrix2, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	std::cout << "Test CalculateLogNormalPDF, CalculateLogNormalCDF and CalculateLogNormalInverseCDF" << std::endl;
	mu = 0.0;
	sigma = 0.25;
	CalculateLogNormalPDF(input_matrix1, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateLogNormalCDF(input_matrix1, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateLogNormalInverseCDF(input_matrix2, mu, sigma, output_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	std::cout << std::endl;
	output_file << std::endl;

	return timer.GetTotalTime();
}
