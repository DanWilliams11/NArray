#pragma once

#include "../Core/NArrayDefinition.h"

namespace NArray
{
	// function prototypes
	void TestSerialisation(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity);
	void TestSerialisation(NArraySupport::StorageOption storage_option, std::fstream& output_file, bool verbosity, bool uniform);
};
