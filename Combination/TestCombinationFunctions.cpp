#include <complex>
#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Serialisation/SerialisationOperators.h"
#include "../Testing/Timer.h"
#include "ArithmeticCombiners.h"
#include "CombinationFunctions.h"
#include "TestCombinationFunctions.h"

double NArray::TestCombinationFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool multi_threaded)
{
	std::cout << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::cout << "Combination Functions" << std::endl << std::endl;
	output_file << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Combination Functions" << std::endl << std::endl;

	Mask multi_threading_mask(c_matrix_order, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::not_use);
	Mask* multi_threading_mask_ptr = nullptr;
	if (multi_threaded)
		multi_threading_mask_ptr = &multi_threading_mask;

	Timer timer;
	int num_rows = 1000;
	int num_cols = 1000;
	Shape shape(c_matrix_order, num_rows, num_cols);
	std::string shape_string = std::to_string(num_rows) + " X " + std::to_string(num_cols);

	std::cout << "Combination Functions on " << shape_string << " Matrix of double (CombineValues using Add, Subtract, Multiply, Divide)" << std::endl;
	typedef double stored_type1;
	typedef NArrayType<stored_type1>::Matrix storage_type1;
	storage_type1 matrix1(storage_option, shape, stored_type1(0.0), true);
	storage_type1 matrix2(matrix1);
	CombineValues(matrix1, matrix2, Add<stored_type1>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix1, matrix2, Subtract<stored_type1>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix1, matrix2, Multiply<stored_type1>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix1, matrix2, Divide<stored_type1>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);

	std::cout << "Combination Functions on " << shape_string << " Matrix of complex<double> (CombineValues using Add, Subtract, Multiply, Divide)" << std::endl;
	typedef std::complex<stored_type1> stored_type2;
	typedef NArrayType<stored_type2>::Matrix storage_type2;
	storage_type2 matrix3(storage_option, shape, stored_type2(0.0, 0.0), true);
	storage_type2 matrix4(matrix3);
	CombineValues(matrix3, matrix4, Add<stored_type2>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix3, matrix4, Subtract<stored_type2>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix3, matrix4, Multiply<stored_type2>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);
	CombineValues(matrix3, matrix4, Divide<stored_type2>(), multi_threading_mask_ptr, &timer, &output_file, verbosity);

	num_rows = 10;
	num_cols = 10;
	shape.ReShape(c_matrix_order, num_rows, num_cols);
	shape_string = std::to_string(num_rows) + " X " + std::to_string(num_cols);
	matrix1.ReShape(storage_option, shape, stored_type1(0.0), true);
	matrix3.ReShape(storage_option, shape, stored_type2(0.0, 0.0), true);
	matrix2 = matrix1;
	matrix4 = matrix3;
	std::cout << "Combination Functions on " << shape_string << " Matrix of double (operator+, operator-, operator*, operator/)" << std::endl;
	output_file << "matrix1 + matrix2 = " << std::endl << (matrix1 + matrix2);
	output_file << "matrix1 - matrix2 = " << std::endl << (matrix1 - matrix2);
	output_file << "matrix1 * matrix2 = " << std::endl << (matrix1 * matrix2);
	output_file << "matrix1 / matrix2 = " << std::endl << (matrix1 / matrix2);
	std::cout << "Combination Functions on " << shape_string << " Matrix of complex<double> (operator+, operator-, operator*, operator/)" << std::endl;
	output_file << "matrix3 + matrix4 = " << std::endl << (matrix3 + matrix4);
	output_file << "matrix3 - matrix4 = " << std::endl << (matrix3 - matrix4);
	output_file << "matrix3 * matrix4 = " << std::endl << (matrix3 * matrix4);
	output_file << "matrix3 / matrix4 = " << std::endl << (matrix3 / matrix4);

	std::cout << std::endl;
	output_file << std::endl;

	return timer.GetTotalTime();
}
