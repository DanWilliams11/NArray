#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Testing/Timer.h"
#include "TestUsefulFunctions.h"
#include "UsefulFunctions.h"

double NArray::TestUsefulFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool multi_threaded)
{
	output_file << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Useful Functions" << std::endl << std::endl;
	
	Mask multi_threading_mask(c_matrix_order, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::not_use);
	Mask* multi_threading_mask_ptr = nullptr;
	if (multi_threaded)
		multi_threading_mask_ptr = &multi_threading_mask;

	Timer timer;
	int num_rows = 1000;
	int num_cols = 1000;
	Shape shape(c_matrix_order, num_rows, num_cols);

	typedef int stored_type;
	typedef NArrayType<stored_type>::Matrix storage_type;
	storage_type output_matrix(storage_option, shape, stored_type(0), false);

	stored_type start_val = 1;
	stored_type step_val = 1;
	storage_type input_matrix(storage_option, shape, stored_type(0), true);
	MakeRamped<storage_type, stored_type>(input_matrix, start_val, step_val, &timer, &output_file, verbosity);

	stored_type min = 1;
	stored_type max = 10;
	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeTranspose<storage_type, stored_type>(input_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeIdentity<storage_type, stored_type>(input_matrix, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeSymmetric<storage_type, stored_type>(input_matrix, NArraySupport::LTR, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeSymmetric<storage_type, stored_type>(input_matrix, NArraySupport::UTR, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeTriangleZero<storage_type, stored_type>(input_matrix, NArraySupport::LTR, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeTriangleOne<storage_type, stored_type>(input_matrix, NArraySupport::UTR, multi_threading_mask_ptr, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	MakeTriangleValue<storage_type, stored_type>(input_matrix, NArraySupport::DIAG, 999, &timer, &output_file, verbosity);

	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	output_matrix = Transpose<storage_type, stored_type>(input_matrix, multi_threading_mask_ptr, storage_option, &timer, &output_file, verbosity);

	num_rows = 5;
	num_cols = 5;
	shape.ReShape(c_matrix_order, num_rows, num_cols);
	input_matrix.ReShape(storage_option, shape, stored_type(0), true);
	MakeRandom(input_matrix, min, max, &timer, &output_file, verbosity);
	output_matrix = DiffPairwiseColumns<storage_type, stored_type, NArrayType<bool>::Matrix, bool>(input_matrix, storage_option, &timer, &output_file, verbosity);

	output_file << std::endl;

	return timer.GetTotalTime();
}
