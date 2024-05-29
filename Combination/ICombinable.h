#pragma once

namespace NArray
{
	// classes
	template <class U> class ICombinable abstract
	{
		// member functions
	public:
		virtual U ApplyCombiner(const U& in1, const U& in2) const = 0;
		virtual std::string GetName() const = 0;
	};

	template <class U> class IArithmeticCombinable abstract : public ICombinable<U>
	{
		// member functions
	public:
		virtual U GetIdentityElement() const = 0;
	};
}
