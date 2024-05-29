#pragma once

#include "../Core/NArrayDefinition.h"

namespace NArray
{
	// function prototypes
	void CompareTimes(NArraySupport::StorageOption storage_option, std::fstream& output_file, double single_threaded_time, double multi_threaded_time);
	void GenerateReferenceTimes(std::fstream& output_file);
	void TestAllFunctions(bool verbosity);
}
