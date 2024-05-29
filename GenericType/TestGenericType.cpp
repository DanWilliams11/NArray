#include <fstream>

#include "../Arithmetic/ArithmeticFunctions.h"
#include "../Combination/ArithmeticCombiners.h"
#include "../Core/NArrayImplementation.h"
#include "../Serialisation/SerialisationOperators.h"
#include "../Testing/Timer.h"
#include "../Useful/UsefulFunctions.h"
#include "GenericType.h"
#include "TestGenericType.h"

void NArray::TestGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "GenericType" << std::endl << std::endl;

	TestHomogeneousGenericType(storage_option, output_file, verbosity);
	TestHeterogeneousGenericType(storage_option, output_file, verbosity);
}

void NArray::TestHomogeneousGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	output_file << "*** Testing Homogeneous GenericType" << std::endl << std::endl;

	typedef GenericType stored_type1;
	typedef std::complex<double> stored_type2;
	typedef NArrayType<stored_type1>::Array4D storage_type1;
	typedef NArrayType<stored_type2>::Array4D storage_type2;

	Timer timer;
	Shape shape(c_array4D_order, 2, 2, 2, 2);
	stored_type1* values_generictype = nullptr;
	stored_type2* values_complex = nullptr;
	storage_type1 array_generictype;
	storage_type2 array_complex;
	stored_type1 empty_val = stored_type2();
	stored_type1 start_val = stored_type2(0.0, 0.0);
	stored_type1 step_val = stored_type2(0.1, 0.1);
	stored_type1 end_val = stored_type2(999.999, 999.999);

	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_generictype = new stored_type1[shape.GetTotalSize()];
		for (int i = 0; i < shape.GetTotalSize(); i++)
			values_generictype[i] = empty_val;
		values_complex = new stored_type2[shape.GetTotalSize()];
		array_generictype.ReShape(storage_option, shape, values_generictype, true);
		array_complex.ReShape(storage_option, shape, values_complex, true);
	}
	else
	{
		array_generictype.ReShape(storage_option, shape, empty_val, true);
		array_complex.ReShape(storage_option, shape, empty_val, true);
	}
	array_generictype.PrettyPrint(output_file, verbosity, "Array GenericType");
	array_complex.PrettyPrint(output_file, verbosity, "Array complex");

	output_file << "Copy-construction" << std::endl;
	storage_type1 array_generictype_copy(array_generictype);
	storage_type2 array_complex_copy(array_complex);
	array_generictype_copy.PrettyPrint(output_file, verbosity, "Copy of Array GenericType");
	array_complex_copy.PrettyPrint(output_file, verbosity, "Copy of Array complex");

	output_file << "Useful Functions" << std::endl;
	MakeRamped(array_generictype, start_val, step_val, &timer, &output_file, verbosity);
	MakeZero<storage_type1, stored_type1>(array_generictype, nullptr, &timer, &output_file, verbosity);
	MakeOne<storage_type1, stored_type1>(array_generictype, nullptr, &timer, &output_file, verbosity);
	MakeValue(array_generictype, end_val, &timer, &output_file, verbosity);
	MakeRandom<storage_type1, stored_type2>(array_generictype, start_val, end_val, &timer, &output_file, verbosity);
	StaticCastValues<storage_type1, stored_type1, storage_type2, stored_type2>(array_generictype, array_complex, nullptr, &timer, &output_file, verbosity);

	output_file << "Arithmetic Functions" << std::endl;
	MakeRamped(array_generictype, start_val, step_val, &timer, &output_file, verbosity);
	CalculateArguments<storage_type1, stored_type2, storage_type1, stored_type1>(array_generictype, array_generictype, nullptr, &timer, &output_file, verbosity);
	MakeRamped(array_generictype, start_val, step_val, &timer, &output_file, verbosity);
	CalculateModuli<storage_type1, stored_type2, storage_type1, stored_type1>(array_generictype, array_generictype, nullptr, &timer, &output_file, verbosity);

	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_generictype;
		delete[] values_complex;
	}
}

void NArray::TestHeterogeneousGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	output_file << "*** Testing Heterogeneous GenericType" << std::endl << std::endl;

	typedef GenericType stored_type;
	typedef NArrayType<stored_type>::Vector storage_type;

	Shape shape(c_vector_order, 6);
	stored_type* values_generictype1 = nullptr;
	stored_type* values_generictype2 = nullptr;
	storage_type array_generictype1;
	storage_type array_generictype2;
	stored_type empty_val = stored_type();

	output_file << "Construction" << std::endl;
	if (storage_option == NArraySupport::ContiguousExternal)
	{
		values_generictype1 = new stored_type[shape.GetTotalSize()];
		values_generictype2 = new stored_type[shape.GetTotalSize()];
		array_generictype1.ReShape(storage_option, shape, values_generictype1, true);
		array_generictype2.ReShape(storage_option, shape, values_generictype2, true);
	}
	else
	{
		array_generictype1.ReShape(storage_option, shape, empty_val, true);
		array_generictype2.ReShape(storage_option, shape, empty_val, true);
	}
	array_generictype1.PrettyPrint(output_file, verbosity, "Array GenericType");
	array_generictype2.PrettyPrint(output_file, verbosity, "Array GenericType");

	output_file << "Copy-construction" << std::endl;
	storage_type array_generictype1_copy(array_generictype1);
	storage_type array_generictype2_copy(array_generictype2);
	array_generictype1_copy.PrettyPrint(output_file, verbosity, "Copy of Array GenericType");
	array_generictype2_copy.PrettyPrint(output_file, verbosity, "Copy of Array GenericType");

	output_file << "Population" << std::endl;
	array_generictype1[1][1] = false;
	array_generictype1[2][1] = 'a';
	array_generictype1[3][1] = 1;
	array_generictype1[4][1] = 0.001;
	array_generictype1[5][1] = std::complex<double>(0.001, 0.001);
	array_generictype1[6][1] = std::string("wibble");
	array_generictype1.PrettyPrint(output_file, verbosity, "Array GenericType (mix of bool, char, int, double, std::complex<double> and std::string)");
	array_generictype1[6][1] = false;
	array_generictype1[5][1] = 'a';
	array_generictype1[4][1] = 1;
	array_generictype1[3][1] = 0.001;
	array_generictype1[2][1] = std::complex<double>(0.001, 0.001);
	array_generictype1[1][1] = std::string("wibble");
	array_generictype1.PrettyPrint(output_file, verbosity, "Array GenericType (having changed the types at run-time)");
	array_generictype2[1][1] = true;
	array_generictype2[2][1] = 'z';
	array_generictype2[3][1] = 999;
	array_generictype2[4][1] = 999.999;
	array_generictype2[5][1] = std::complex<double>(999.999, 999.999);
	array_generictype2[6][1] = std::string("wobble");
	array_generictype2.PrettyPrint(output_file, verbosity, "Array GenericType (mix of bool, char, int, double, std::complex<double> and std::string)");
	array_generictype2[6][1] = true;
	array_generictype2[5][1] = 'z';
	array_generictype2[4][1] = 999;
	array_generictype2[3][1] = 999.999;
	array_generictype2[2][1] = std::complex<double>(999.999, 999.999);
	array_generictype2[1][1] = std::string("wobble");
	array_generictype2.PrettyPrint(output_file, verbosity, "Array GenericType (having changed the types at run-time)");
	output_file << "Sum of the two arrays:" << std::endl << array_generictype1 + array_generictype2;

	output_file << std::endl;

	if (storage_option == NArraySupport::ContiguousExternal)
	{
		delete[] values_generictype1;
		delete[] values_generictype2;
	}
}
