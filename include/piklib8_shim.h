#pragma once

#ifdef PIKLIB8SHIM_EXPORTS
	#define PIKLIB8SHIM_API __declspec(dllexport)
#else
	#define PIKLIB8SHIM_API __declspec(dllimport)
#endif

extern "C"
{
	//Compress string from the structure
	PIKLIB8SHIM_API char *piklib_CLZWCompression2_compress(char *input_string, int input_size, int *output_size);

	//Decompress string from the structure
	PIKLIB8SHIM_API char *piklib_CLZWCompression2_decompress(char *input_string, int input_size);

	//Free memory allocated by C++ new operator
	PIKLIB8SHIM_API void piklib_CLZWCompression2_deallocate(char *output_string);
}
