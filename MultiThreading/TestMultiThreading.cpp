#include <fstream>

#include "../Arithmetic/ArithmeticFunctions.h"
#include "../Core/NArrayImplementation.h"
#include "../Testing/Timer.h"
#include "../Useful/UsefulFunctions.h"
#include "../Visualisation/VisualisationFunctions.h"
#include "MultiThreading.h"
#include "TestMultiThreading.h"

double NArray::TestMultiThreading(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool multi_threaded)
{
	std::cout << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::cout << "Convolution of a 3D image with a 3D kernel" << std::endl << std::endl;
	output_file << "*** Testing " << NArraySupport::MultiThreadingStr(multi_threaded) << " for " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Convolution of a 3D image with a 3D kernel" << std::endl << std::endl;

	Mask multi_threading_mask(c_array3D_order, Mask::not_use, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::use_all);
	Mask* multi_threading_mask_ptr = nullptr;
	if (multi_threaded)
		multi_threading_mask_ptr = &multi_threading_mask;

	Timer timer;
	int image_width = 10;
	int image_height = 10;
	int image_depth = 10;
	int kernel_width = 3;
	int kernel_height = 3;
	int kernel_depth = 3;
	Shape image_shape(c_array3D_order, image_depth, image_height, image_width);
	Shape kernel_shape(c_array3D_order, kernel_depth, kernel_height, kernel_width);

	typedef double stored_type;
	typedef NArrayType<stored_type>::Array3D storage_type;
	storage_type image_in(storage_option, image_shape, stored_type(0.0), true);
	storage_type kernel(storage_option, kernel_shape, stored_type(0.0), true);
	storage_type image_out(storage_option, image_shape, stored_type(0.0), true);

	MakeOne<storage_type, stored_type>(image_in, nullptr, &timer, &output_file, verbosity);
	MakeOne<storage_type, stored_type>(kernel, nullptr, &timer, &output_file, verbosity);
	CalculateConvolution<storage_type, stored_type>(image_in, kernel, image_out, multi_threading_mask_ptr, &timer, &output_file, verbosity);
	MapValues<storage_type, stored_type>(image_out, 0.0, 15.0, nullptr, &timer, &output_file, verbosity);

	std::cout << "The input image..." << std::endl;
	Visualise3D<storage_type, stored_type>(image_in, NArraySupport::UseDataToSize);
	std::cout << "has been convolved with a kernel..." << std::endl;
	Visualise3D<storage_type, stored_type>(kernel, NArraySupport::UseDataToSize);
	std::cout << "to produce the output image..." << std::endl;
	Visualise3D<storage_type, stored_type>(image_out, NArraySupport::UseDataToSize);

	std::cout << std::endl;
	output_file << std::endl;

	return timer.GetTotalTime();
}
