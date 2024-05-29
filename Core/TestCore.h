#pragma once

#include "NArrayDefinition.h"

namespace NArray
{
	// function prototypes
	void TestCore(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestScalars(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestVectors(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestMatrices(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestArray3D(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
}
