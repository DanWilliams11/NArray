#pragma once

#include "../Core/Operator.h"
#include "ICommunicable.h"

namespace NArray
{
	// class for communicating between the objects in an n-dimensional array - tested
	// this will be used if the array objects do not inherit from ICommunicable
	template <class S, class V, bool = std::is_base_of<ICommunicable, V>::value> class Communicate : public MultiThreadedOperator<S, V>
	{
		// not currently implemented
	};

	// partial specialisation of class for communicating between the objects in an n-dimensional array - tested
	// this will be used if the array objects inherit from ICommunicable
	template <class S, class V> class Communicate<S, V, true> : public MultiThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			V& obj = const_cast<V&>(value);
			obj.StartCommunicating();
			return true;
		}
	};
}
