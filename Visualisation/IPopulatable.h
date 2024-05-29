#pragma once

namespace NArray
{
	// interface for ensuring an object can be populated
	template <class V> class IPopulatable abstract
	{
		// member functions
	public:
		virtual void Populate(const Index& index, const V& value) = 0;
	};
}
