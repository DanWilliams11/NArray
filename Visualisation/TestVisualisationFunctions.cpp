#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Useful/UsefulFunctions.h"
#include "../VisualisationExamples/JuliaSet.h"
#include "../VisualisationExamples/MandelbrotSet.h"
#include "TestVisualisationFunctions.h"
#include "VisualisationFunctions.h"

void NArray::TestVisualisationFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file)
{
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Visualisation Functions" << std::endl << std::endl;

	Mask multi_threading_mask(c_array3D_order, Mask::not_use, Mask::not_use, Mask::use_all);
	multi_threading_mask.PutMaskAtDim(0, Mask::not_use);

	typedef double stored_type;
	stored_type start_val = 1.0;
	stored_type end_val = 10.0;
	stored_type step_val = 1.0;

	typedef NArrayType<stored_type>::Scalar storage_type0D;
	storage_type0D image_0D(storage_option, start_val);
	MakeValue(image_0D, end_val);
	Visualise0D<storage_type0D, stored_type>(image_0D, NArraySupport::UseDataToSize);

	typedef NArrayType<stored_type>::Vector storage_type1D;
	int image_width = 10;
	Shape image_shape(c_vector_order, image_width);
	storage_type1D image_1D(storage_option, image_shape, start_val, true);
	MakeRamped(image_1D, start_val, step_val);
	Visualise1D<storage_type1D, stored_type>(image_1D, NArraySupport::UseDataToSize);

	typedef NArrayType<stored_type>::Matrix storage_type2D;
	int image_height = 10;
	image_shape.ReShape(c_matrix_order, image_height, image_width);
	storage_type2D image_2D(storage_option, image_shape, start_val, true);
	MakeTriangleValue(image_2D, NArraySupport::UTR, end_val);
	Visualise2D<storage_type2D, stored_type>(image_2D, NArraySupport::UseDataToSize);

	typedef NArrayType<stored_type>::Array3D storage_type3D;
	int image_depth = 10;
	image_shape.ReShape(c_array3D_order, image_depth, image_height, image_width);
	storage_type3D image_3D(storage_option, image_shape, start_val, true);
	MakeRandom(image_3D, start_val, end_val);
	Visualise3D<storage_type3D, stored_type>(image_3D, NArraySupport::UseDataToSize);

	image_3D.ReShape(storage_option, image_shape, end_val, true);
	Visualiser3D<storage_type3D, stored_type> visualiser_3D(image_shape);
	visualiser_3D.StartDraw();
	MakeZero<storage_type3D, stored_type>(image_3D, &multi_threading_mask);
	MakeTriangleValue<storage_type3D, stored_type>(image_3D, NArraySupport::LTR, end_val);
	visualiser_3D.ReColour(1.0, 0.0, 0.0);
	ApplySingleThreadedOperator(visualiser_3D, image_3D);
	MakeZero<storage_type3D, stored_type>(image_3D, &multi_threading_mask);
	MakeTriangleValue<storage_type3D, stored_type>(image_3D, NArraySupport::UTR, end_val);
	visualiser_3D.ReColour(0.0, 1.0, 0.0);
	ApplySingleThreadedOperator(visualiser_3D, image_3D);
	MakeZero<storage_type3D, stored_type>(image_3D, &multi_threading_mask);
	MakeTriangleValue<storage_type3D, stored_type>(image_3D, NArraySupport::DIAG, end_val);
	visualiser_3D.ReColour(0.0, 0.0, 1.0);
	ApplySingleThreadedOperator(visualiser_3D, image_3D);
	visualiser_3D.EndDraw();

	typedef std::complex<double> stored_type_in;
	typedef ChaoticDynamicalSystems::JuliaSet<stored_type_in> stored_type_out1;
	typedef ChaoticDynamicalSystems::MandelbrotSet<stored_type_in> stored_type_out2;
	image_width = 1000;
	image_height = 1000;
	for (int i = 0; i < 2; i++)
	{
		bool use_transform = true;
		if (i == 1)
			use_transform = false;
		stored_type_out1::Start(storage_option, image_width, image_height, use_transform, output_file, &multi_threading_mask);
		stored_type_out2::Start(storage_option, image_width, image_height, use_transform, output_file, &multi_threading_mask);
	}
}
