#pragma once

#include <string>

namespace NArray
{
	// classes
	template <class U> class IComparable abstract
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const = 0;
		virtual std::string GetName() const = 0;
	};

	template <class U> class ILogicalComparable abstract : public IComparable<U>
	{
		// member functions
	public:
		virtual bool GetIdentityElement() const = 0;
	};
}
