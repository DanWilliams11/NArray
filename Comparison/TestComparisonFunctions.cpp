#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Testing/Timer.h"
#include "ComparisonFunctions.h"
#include "LogicalComparators.h"
#include "RelationalComparators.h"
#include "TestComparisonFunctions.h"

double NArray::TestComparisonFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file)
{
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Comparison Functions" << std::endl << std::endl;

	Timer timer;
	int num_rows = 1000;
	int num_cols = 1000;
	Shape shape(c_matrix_order, num_rows, num_cols);

	typedef int stored_type1;
	typedef NArrayType<stored_type1>::Matrix storage_type1;
	storage_type1 matrix1(storage_option, shape, stored_type1(0), true);
	storage_type1 matrix2(matrix1);
	CompareValues(matrix1, matrix2, Equal<stored_type1>(), &timer, &output_file);
	CompareValues(matrix1, matrix2, NotEqual<stored_type1>(), &timer, &output_file);
	CompareValues(matrix1, matrix2, LessThan<stored_type1>(), &timer, &output_file);
	CompareValues(matrix1, matrix2, LessThanOrEqual<stored_type1>(), &timer, &output_file);
	CompareValues(matrix1, matrix2, GreaterThan<stored_type1>(), &timer, &output_file);
	CompareValues(matrix1, matrix2, GreaterThanOrEqual<stored_type1>(), &timer, &output_file);

	typedef bool stored_type2;
	typedef NArrayType<stored_type2>::Matrix storage_type2;
	storage_type2 matrix3(storage_option, shape, stored_type2(false), true);
	storage_type2 matrix4(matrix3);
	CompareValues(matrix3, matrix4, And<stored_type2>(), &timer, &output_file);
	CompareValues(matrix3, matrix4, Nand<stored_type2>(), &timer, &output_file);
	CompareValues(matrix3, matrix4, Or<stored_type2>(), &timer, &output_file);
	CompareValues(matrix3, matrix4, Nor<stored_type2>(), &timer, &output_file);
	CompareValues(matrix3, matrix4, Xor<stored_type2>(), &timer, &output_file);

	typedef bool stored_type3;
	typedef NArrayType<stored_type3>::Matrix storage_type3;
	storage_type3 matrix5(storage_option, shape, stored_type3(false), false);
	CompareValues(matrix5, And<stored_type3>(), &timer, &output_file);
	CompareValues(matrix5, Nand<stored_type3>(), &timer, &output_file);
	CompareValues(matrix5, Or<stored_type3>(), &timer, &output_file);
	CompareValues(matrix5, Nor<stored_type3>(), &timer, &output_file);
	CompareValues(matrix5, Xor<stored_type3>(), &timer, &output_file);

	output_file << "matrix1 == matrix2 = " << (matrix1 == matrix2) << std::endl;
	output_file << "matrix1 != matrix2 = " << (matrix1 != matrix2) << std::endl;
	output_file << "matrix1 < matrix2 = " << (matrix1 < matrix2) << std::endl;
	output_file << "matrix1 <= matrix2 = " << (matrix1 <= matrix2) << std::endl;
	output_file << "matrix1 > matrix2 = " << (matrix1 > matrix2) << std::endl;
	output_file << "matrix1 >= matrix2 = " << (matrix1 >= matrix2) << std::endl;
	output_file << "matrix3 && matrix4 = " << (matrix3 && matrix4) << std::endl;
	output_file << "matrix3 || matrix4 = " << (matrix3 || matrix4) << std::endl;

	output_file << std::endl;

	return timer.GetTotalTime();
}
