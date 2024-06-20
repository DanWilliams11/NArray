#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <crtdbg.h>
#include <stdexcept>
#include <string>
#include <Windows.h>

#include "Testing/UnitTests.h"

int main()
{
	int ret = 0;
	// _CrtSetBreakAlloc(151); // trap the created memory leak - see below
	try
	{
		// int* memory_leak = new int; // create a memory leak to ensure testing for memory leaks works
		// *memory_leak = 0;

		NArray::TestAllFunctions();
	}
	catch (std::exception& error)
	{
		std::string error_str("Exception Caught: " + std::string(typeid(error).name()) + ": " + std::string(error.what()) + '\n');
		OutputDebugStringA(error_str.c_str());
		ret = 999;
	}
	_CrtDumpMemoryLeaks();
	return ret;
}
