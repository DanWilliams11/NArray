#include <fstream>

#include "../Core/NArrayImplementation.h"
#include "../Testing/Timer.h"
#include "TestSerialisation.h"

void NArray::TestSerialisation(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity)
{
	for (int i = 0; i < 2; i++)
	{
		bool uniform = false;
		if (i == 1)
			uniform = true;
		
		TestSerialisation(storage_option, output_file, verbosity, uniform);

		output_file << std::endl;
	}
}

void NArray::TestSerialisation(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool uniform)
{
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::string file_str;
	if (uniform)
	{
		output_file << "Serialisation of uniform n-dimensional array" << std::endl << std::endl;
		file_str = "Serialisation/uniform.txt";
	}
	else
	{
		output_file << "Serialisation of non-uniform n-dimensional array" << std::endl << std::endl;
		file_str = "Serialisation/non_uniform.txt";
	}

	Timer timer;
	bool in = true;
	bool out = false;
	Shape small_shape(c_array3D_order, 10, 1000, 30);
	Shape big_shape(c_array3D_order, 10, 10000, 30);

	typedef int stored_type;
	typedef NArrayType<stored_type>::Array3D storage_type;

	for (int i = 0; i < 2; i++)
	{
		bool reindex = false;
		if (i == 1)
			reindex = true;

		Shape& shape = big_shape;
		if (reindex)
		{
			shape = small_shape; // indexes take up a lot of room!
			output_file << std::endl << "The array is now smaller because reindexing is on and indexes take up a lot of room - modify this according to available memory" << std::endl << std::endl;
			if (!uniform)
				output_file << "This will take a long time because even though the array is smaller, it is non-uniform and the indexes need to be recalculated on-the-fly" << std::endl << std::endl;
		}

		{
			timer.StartTimer();
			storage_type data_out(storage_option, shape, stored_type(1), reindex); // data_out will now be uniform
			if (!uniform) // make data_out non-uniform
			{
				data_out[2].PutSize(data_out, 5, stored_type(2), reindex);
				data_out[2][2].PutSize(data_out, 4, stored_type(2), reindex);
				data_out[2][4].PutSize(data_out, 6, stored_type(2), reindex);
				data_out[4].PutSize(data_out, 20, stored_type(2), reindex);
				data_out[4][2].PutSize(data_out, 2, stored_type(2), reindex);
				data_out[4][4].PutSize(data_out, 12, stored_type(2), reindex);
				data_out[4][11].PutSize(data_out, 21, stored_type(2), reindex);
			}
			timer.StopTimer();
			output_file << "Constructing the output array with re-indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl;

			timer.StartTimer();
			std::fstream serialisation_out(file_str, std::ios_base::out);
			if (uniform)
				data_out.SerialiseUniform(storage_option, serialisation_out, out, reindex);
			else
				data_out.SerialiseNonUniform(data_out, serialisation_out, out, reindex);
			serialisation_out.close();
			timer.StopTimer();
			output_file << "Number of elements in output array is " << data_out.GetTotalSize() << std::endl;
			output_file << "Serialisation out with re-indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl;
			data_out.PrettyPrint(output_file, verbosity, "Data Out:", true);
		}

		{
			timer.StartTimer();
			storage_type data_in;
			std::fstream serialisation_in(file_str, std::ios_base::in);
			if (uniform)
				data_in.SerialiseUniform(storage_option, serialisation_in, in, reindex);
			else
				data_in.SerialiseNonUniform(data_in, serialisation_in, in, reindex);
			serialisation_in.close();
			timer.StopTimer();
			output_file << "Number of elements in input array is " << data_in.GetTotalSize() << std::endl;
			output_file << "Serialisation in with re-indexing " << NArraySupport::ReIndexStr(reindex) << " took " << timer.GetTime() << std::endl;
			data_in.PrettyPrint(output_file, verbosity, "Data In:", true);
		}
	}
}
