#pragma once

#include "../Core/NArrayDefinition.h"

namespace NArray
{
	// serialisation non-member operators

	template <class U> std::ostream& operator <<(std::ostream& output_stream, const NArrayAxiomatic<U>& in)
	{
		in.Write(output_stream);
		return output_stream;
	}

	template <class U> std::istream& operator >>(std::istream& input_stream, NArrayAxiomatic<U>& in)
	{
		in.Read(input_stream);
		return input_stream;
	}

	template <class T, class U> std::ostream& operator <<(std::ostream& output_stream, const NArrayRecursive<T, U>& in)
	{
		in.Write(output_stream);
		return output_stream;
	}

	template <class T, class U> std::istream& operator >>(std::istream& input_stream, NArrayRecursive<T, U>& in)
	{
		in.Read(input_stream);
		return input_stream;
	}
}
