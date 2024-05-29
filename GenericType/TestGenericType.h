#pragma once

#include "../Core/NArrayDefinition.h"

namespace NArray
{
	// function prototypes
	void TestGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestHomogeneousGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestHeterogeneousGenericType(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
}
