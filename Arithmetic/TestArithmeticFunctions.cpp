#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Core/NArraySupport.h"
#include "../Testing/Timer.h"
#include "../Useful/UsefulFunctions.h"
#include "ArithmeticFunctions.h"
#include "TestArithmeticFunctions.h"

double NArray::TestArithmeticFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool multi_threaded)
{
	output_file << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Arithmetic Functions" << std::endl << std::endl;

	Mask multi_threading_mask(c_matrix_order, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::not_use);
	Mask* multi_threading_mask_ptr = nullptr;
	if (multi_threaded)
		multi_threading_mask_ptr = &multi_threading_mask;

	Timer timer;
	int num_rows = NArraySupport::c_max_num_threads;
	int num_cols = 1000000;
	Shape shape(c_matrix_order, num_rows, num_cols);

	typedef double stored_type1;
	typedef NArrayType<stored_type1>::Matrix storage_type1;
	storage_type1 matrix1(storage_option, shape, stored_type1(0.0), false);
	MakeRamped<storage_type1, stored_type1>(matrix1, &timer, &output_file, verbosity);
	MakeRamped<storage_type1, stored_type1>(matrix1, 1.0, 2.0, &timer, &output_file, verbosity);
	CalculatePi<storage_type1, stored_type1>(matrix1, &timer, &output_file);
	CalculateSigma<storage_type1, stored_type1>(matrix1, &timer, &output_file);
	OffsetValues<storage_type1, stored_type1>(matrix1, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	OffsetValues<storage_type1, stored_type1>(matrix1, 2.0, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	ScaleValues<storage_type1, stored_type1>(matrix1, 2.0, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	typedef std::complex<stored_type1> stored_type2;
	typedef NArrayType<stored_type2>::Matrix storage_type2;
	storage_type2 matrix2(storage_option, shape, stored_type2(0.0, 0.0), true);
	MakeRamped<storage_type2, stored_type2>(matrix2, &timer, &output_file, verbosity);
	MakeRamped<storage_type2, stored_type2>(matrix2, stored_type2(1.0, 1.0), stored_type2(2.0, 2.0), &timer, &output_file, verbosity);
	CalculateArguments<storage_type2, stored_type2, storage_type1, stored_type1>(matrix2, matrix1, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CalculateModuli<storage_type2, stored_type2, storage_type1, stored_type1>(matrix2, matrix1, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	output_file << std::endl;

	return timer.GetTotalTime();
}
