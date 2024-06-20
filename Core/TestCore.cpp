#include <fstream>

#include "../Testing/Timer.h"
#include "../Useful/UsefulFunctions.h"
#include "NArrayImplementation.h"
#include "TestCore.h"

void NArray::TestCore(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	std::cout << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::cout << "Core" << std::endl << std::endl;
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Core" << std::endl << std::endl;

	TestScalars(storage_option, output_file, verbosity);
	TestVectors(storage_option, output_file, verbosity);
	TestMatrices(storage_option, output_file, verbosity);
	TestArray3D(storage_option, output_file, verbosity);
}

void NArray::TestScalars(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	std::cout << "*** Testing Scalars" << std::endl << std::endl;
	output_file << "*** Testing Scalars" << std::endl << std::endl;

	Timer timer;
	bool* values_scalar_bool = nullptr;
	char* values_scalar_char = nullptr;
	NArrayType<bool>::Scalar scalar_bool;
	NArrayType<char>::Scalar scalar_char;

	std::cout << "Construction" << std::endl;
	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_scalar_bool = new bool[c_scalar_size];
		values_scalar_char = new char[c_scalar_size];
		scalar_bool.ReShape(storage_option, values_scalar_bool);
		scalar_char.ReShape(storage_option, values_scalar_char);
	}
	else
	{
		scalar_bool.ReShape(storage_option, false);
		scalar_char.ReShape(storage_option, '0');
	}
	scalar_bool.PrettyPrint(output_file, verbosity, "Scalar bool");
	scalar_char.PrettyPrint(output_file, verbosity, "Scalar char");

	std::cout << "Copy-construction" << std::endl;
	output_file << "Copy-construction" << std::endl; 
	NArrayType<bool>::Scalar scalar_bool_copy(scalar_bool);
	NArrayType<char>::Scalar scalar_char_copy(scalar_char);
	scalar_bool_copy.PrettyPrint(output_file, verbosity, "Copy of Scalar bool");
	scalar_char_copy.PrettyPrint(output_file, verbosity, "Copy of Scalar char");

	std::cout << "Population" << std::endl;
	output_file << "Population" << std::endl;
	scalar_bool[1] = true;
	scalar_char[1] = 'a';
	scalar_bool.PrettyPrint(output_file, verbosity, "Scalar bool after setting value using member access operator");
	scalar_char.PrettyPrint(output_file, verbosity, "Scalar char after setting value using member access operator");
	bool* b = new bool[scalar_bool.GetTotalSize()];
	char* c = new char[scalar_char.GetTotalSize()];
	scalar_bool.Write(b);
	scalar_char.Write(c);
	scalar_bool.PrettyPrint(output_file, verbosity, "Scalar bool after construction of a C-style array and subseqent linearisation to the array");
	scalar_char.PrettyPrint(output_file, verbosity, "Scalar char after construction of a C-style array and subseqent linearisation to the array");
	scalar_bool_copy.Read(b);
	scalar_char_copy.Read(c);
	delete[] b;
	delete[] c;
	scalar_bool_copy.PrettyPrint(output_file, verbosity, "Copy of Scalar bool after reading from the linear array and subsequent destruction of the array");
	scalar_char_copy.PrettyPrint(output_file, verbosity, "Copy of Scalar char after reading from the linear array and subsequent destruction of the array");

	std::cout << "Useful Functions (MakeZero, MakeOne, MakeValue, MakeRandom, StaticCastValues" << std::endl;
	output_file << "Useful Functions" << std::endl;
	MakeZero<NArrayType<bool>::Scalar, bool>(scalar_bool, nullptr, &timer, &output_file, verbosity);
	MakeOne<NArrayType<bool>::Scalar, bool>(scalar_bool, nullptr, &timer, &output_file, verbosity);
	MakeValue(scalar_bool, true, &timer, &output_file, verbosity);
	MakeRandom(scalar_bool, false, true, &timer, &output_file, verbosity);
	StaticCastValues<NArrayType<bool>::Scalar, bool, NArrayType<char>::Scalar, char>(scalar_bool, scalar_char, nullptr, &timer, &output_file, verbosity);

	std::cout << std::endl;
	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_scalar_bool;
		delete[] values_scalar_char;
	}
}

void NArray::TestVectors(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	std::cout << "*** Testing Vectors" << std::endl << std::endl;
	output_file << "*** Testing Vectors" << std::endl << std::endl;

	Timer timer;
	Shape shape(c_vector_order, 2);
	char* values_vector_char = nullptr;
	int* values_vector_int = nullptr;
	NArrayType<char>::Vector vector_char;
	NArrayType<int>::Vector vector_int;

	std::cout << "Construction" << std::endl;
	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_vector_char = new char[shape.GetTotalSize()];
		values_vector_int = new int[shape.GetTotalSize()];
		vector_char.ReShape(storage_option, shape, values_vector_char, true);
		vector_int.ReShape(storage_option, shape, values_vector_int, true);
	}
	else
	{
		vector_char.ReShape(storage_option, shape, '0', true);
		vector_int.ReShape(storage_option, shape, 0, true);
	}
	vector_char.PrettyPrint(output_file, verbosity, "Vector char");
	vector_int.PrettyPrint(output_file, verbosity, "Vector int");

	std::cout << "Copy-construction" << std::endl;
	output_file << "Copy-construction" << std::endl; 
	NArrayType<char>::Vector vector_char_copy(vector_char);
	NArrayType<int>::Vector vector_int_copy(vector_int);
	vector_char_copy.PrettyPrint(output_file, verbosity, "Copy of Vector char");
	vector_int_copy.PrettyPrint(output_file, verbosity, "Copy of Vector int");

	std::cout << "Population" << std::endl;
	output_file << "Population" << std::endl;
	vector_char[1][1] = 'a';
	vector_char[2][1] = 'b';
	vector_int[1][1] = 1;
	vector_int[2][1] = 2;
	vector_char.PrettyPrint(output_file, verbosity, "Vector char after setting values using member access operator");
	vector_int.PrettyPrint(output_file, verbosity, "Vector int after setting values using member access operator");
	char* c = new char[vector_char.GetTotalSize()];
	int* i = new int[vector_int.GetTotalSize()];
	vector_char.Write(c);
	vector_int.Write(i);
	vector_char.PrettyPrint(output_file, verbosity, "Vector char after construction of a C-style array and subseqent linearisation to the array");
	vector_int.PrettyPrint(output_file, verbosity, "Vector int after construction of a C-style array and subseqent linearisation to the array");
	vector_char_copy.Read(c);
	vector_int_copy.Read(i);
	delete[] c;
	delete[] i;
	vector_char_copy.PrettyPrint(output_file, verbosity, "Copy of Vector char after reading from the linear array and subsequent destruction of the array");
	vector_int_copy.PrettyPrint(output_file, verbosity, "Copy of Vector int after reading from the linear array and subsequent destruction of the array");

	std::cout << "Useful Functions (MakeRamped, MakeZero, MakeOne, MakeValue, MakeRandom, StaticCastValues)" << std::endl;
	output_file << "Useful Functions" << std::endl;
	MakeRamped(vector_char, 'a', char(1), &timer, &output_file, verbosity);
	MakeZero<NArrayType<char>::Vector, char>(vector_char, nullptr, &timer, &output_file, verbosity);
	MakeOne<NArrayType<char>::Vector, char>(vector_char, nullptr, &timer, &output_file, verbosity);
	MakeValue(vector_char, 'z', &timer, &output_file, verbosity);
	MakeRandom(vector_char, 'a', 'z', &timer, &output_file, verbosity);
	StaticCastValues<NArrayType<char>::Vector, char, NArrayType<int>::Vector, int>(vector_char, vector_int, nullptr, &timer, &output_file, verbosity);

	std::cout << std::endl;
	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_vector_char;
		delete[] values_vector_int;
	}
}

void NArray::TestMatrices(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	std::cout << "*** Testing Matrices" << std::endl << std::endl;
	output_file << "*** Testing Matrices" << std::endl << std::endl;

	Timer timer;
	Shape shape(c_matrix_order, 2, 2);
	int* values_matrix_int = nullptr;
	double* values_matrix_double = nullptr;
	NArrayType<int>::Matrix matrix_int;
	NArrayType<double>::Matrix matrix_double;
		
	std::cout << "Construction" << std::endl;
	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_matrix_int = new int[shape.GetTotalSize()];
		values_matrix_double = new double[shape.GetTotalSize()];
		matrix_int.ReShape(storage_option, shape, values_matrix_int, true);
		matrix_double.ReShape(storage_option, shape, values_matrix_double, true);
	}
	else
	{
		matrix_int.ReShape(storage_option, shape, 0, true);
		matrix_double.ReShape(storage_option, shape, 0.0, true);
	}
	matrix_int.PrettyPrint(output_file, verbosity, "Matrix int");
	matrix_double.PrettyPrint(output_file, verbosity, "Matrix double");

	std::cout << "Copy-construction" << std::endl;
	output_file << "Copy-construction" << std::endl; 
	NArrayType<int>::Matrix matrix_int_copy(matrix_int);
	NArrayType<double>::Matrix matrix_double_copy(matrix_double);
	matrix_int_copy.PrettyPrint(output_file, verbosity, "Copy of Matrix int");
	matrix_double_copy.PrettyPrint(output_file, verbosity, "Copy of Matrix double");

	std::cout << "Useful Functions (MakeRamped, MakeZero, MakeOne, MakeValue, MakeRandom, StaticCastValues)" << std::endl;
	output_file << "Useful Functions" << std::endl;
	MakeRamped(matrix_int, 0, 1, &timer, &output_file, verbosity);
	MakeZero<NArrayType<int>::Matrix, int>(matrix_int, nullptr, &timer, &output_file, verbosity);
	MakeOne<NArrayType<int>::Matrix, int>(matrix_int, nullptr, &timer, &output_file, verbosity);
	MakeValue(matrix_int, 999, &timer, &output_file, verbosity);
	MakeRandom(matrix_int, 0, 999, &timer, &output_file, verbosity);
	StaticCastValues<NArrayType<int>::Matrix, int, NArrayType<double>::Matrix, double>(matrix_int, matrix_double, nullptr, &timer, &output_file, verbosity);

	std::cout << std::endl;
	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_matrix_int;
		delete[] values_matrix_double;
	}
}

void NArray::TestArray3D(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	std::cout << "*** Testing Array3D" << std::endl << std::endl;
	output_file << "*** Testing Array3D" << std::endl << std::endl;

	Timer timer;
	Shape shape(c_array3D_order, 2, 2, 2);
	double* values_array3D_double = nullptr;
	std::complex<double>* values_array3D_complex = nullptr;
	NArrayType<double>::Array3D array3D_double;
	NArrayType<std::complex<double>>::Array3D array3D_complex;

	std::cout << "Construction" << std::endl;
	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_array3D_double = new double[shape.GetTotalSize()];
		values_array3D_complex = new std::complex<double>[shape.GetTotalSize()];
		array3D_double.ReShape(storage_option, shape, values_array3D_double, true);
		array3D_complex.ReShape(storage_option, shape, values_array3D_complex, true);
	}
	else
	{
		array3D_double.ReShape(storage_option, shape, 0.0, true);
		array3D_complex.ReShape(storage_option, shape, std::complex<double>(0.0, 0.0), true);
	}
	array3D_double.PrettyPrint(output_file, verbosity, "Array3D double");
	array3D_complex.PrettyPrint(output_file, verbosity, "Array3D complex");

	std::cout << "Copy-construction" << std::endl;
	output_file << "Copy-construction" << std::endl; 
	NArrayType<double>::Array3D array3D_double_copy(array3D_double);
	NArrayType<std::complex<double>>::Array3D array3D_complex_copy(array3D_complex);
	array3D_double_copy.PrettyPrint(output_file, verbosity, "Copy of Array3D double");
	array3D_complex_copy.PrettyPrint(output_file, verbosity, "Copy of Array3D complex");

	std::cout << "Useful Functions (MakeRamped, MakeZero, MakeOne, MakeValue, MakeRandom, StaticCastValues)" << std::endl;
	output_file << "Useful Functions" << std::endl;
	MakeRamped(array3D_double, 0.0, 0.1, &timer, &output_file, verbosity);
	MakeZero<NArrayType<double>::Array3D, double>(array3D_double, nullptr, &timer, &output_file, verbosity);
	MakeOne<NArrayType<double>::Array3D, double>(array3D_double, nullptr, &timer, &output_file, verbosity);
	MakeValue(array3D_double, 999.999, &timer, &output_file, verbosity);
	MakeRandom(array3D_double, 0.0, 999.999, &timer, &output_file, verbosity);
	StaticCastValues<NArrayType<double>::Array3D, double, NArrayType<std::complex<double>>::Array3D, std::complex<double>>(array3D_double, array3D_complex, nullptr, &timer, &output_file, verbosity);

	std::cout << "Resizing" << std::endl;
	output_file << "Resizing" << std::endl;
	array3D_double.PutSize(array3D_double, 3, 0.0, false);
	array3D_double[1].PutSize(array3D_double, 3, 0.0, false);
	array3D_double[2].PutSize(array3D_double, 3, 0.0, false);
	array3D_double[3].PutSize(array3D_double, 3, 0.0, false);
	array3D_double[1][1].PutSize(array3D_double, 1, 0.0, false);
	array3D_double[1][2].PutSize(array3D_double, 2, 0.0, false);
	array3D_double[1][3].PutSize(array3D_double, 3, 0.0, false);
	array3D_double[2][1].PutSize(array3D_double, 1, 0.0, false);
	array3D_double[2][2].PutSize(array3D_double, 2, 0.0, false);
	array3D_double[2][3].PutSize(array3D_double, 3, 0.0, false);
	array3D_double[3][1].PutSize(array3D_double, 1, 0.0, false);
	array3D_double[3][2].PutSize(array3D_double, 2, 0.0, false);
	array3D_double[3][3].PutSize(array3D_double, 3, 0.0, false);
	array3D_double_copy = array3D_double;
	array3D_double.PrettyPrint(output_file, verbosity, "Array3D double using function 'PutSize'");
	array3D_double_copy.PrettyPrint(output_file, verbosity, "Copy of Array3D double");
	
	std::cout << std::endl;
	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_array3D_double;
		delete[] values_array3D_complex;
	}
}
