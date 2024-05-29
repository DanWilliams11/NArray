#pragma once

#include "../Core/Transform.h"

namespace NArray
{
	// class for transforming the pointer types of the values in an n-dimensional array of one type to an n-dimensional array of different pointer types in the same inheritance hierarchy (upcast / downcast) - tested
	template <class S1, class V1, class S2, class V2> class DynamicCast : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const S1& in, S2& out)
		{
			V2 cast = dynamic_cast<V2>(in.GetPointerToValue(index));

			if (!cast)
				return false;

			out.PutValue(index, cast);
			return true;
		}
	};

	// class for populating the objects in one n-dimensional array using objects from another n-dimensional array - tested
	// the objects in the receiving array must inherit from IPopulatable
	template <class S1, class V1, class S2, class V2> class Populate : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V2& out_value = const_cast<V2&>(out.GetValue(index));
			out_value.Populate(index, value);
			return true;
		}
	};

	// class for transforming an n-dimensional array of one type to an n-dimensional array of a different type - tested
	template <class S1, class V1, class S2, class V2> class StaticCast : public MultiThreadedTransform<S1, V1, S2, V2>
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			V2 cast_value = static_cast<V2>(value);
			out.PutValue(index, cast_value);
			return true;
		}
	};
}
