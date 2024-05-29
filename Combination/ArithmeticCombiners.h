#pragma once

#include "../Core/NArrayDefinition.h"
#include "../GenericType/GenericType.h"
#include "CombinationFunctions.h"
#include "ICombinable.h"

namespace NArray
{
	// class for adding two values - tested
	template <class U> class Add : public IArithmeticCombinable<U>
	{
		// member functions
	public:
		virtual U ApplyCombiner(const U& in1, const U& in2) const
		{
			return in1 + in2;
		}
		virtual U GetIdentityElement() const
		{
			U val;
			GenericType gen_val = val;
			return gen_val.GetAdditiveIdentityElement();
		}
		virtual std::string GetName() const
		{
			return "Add";
		}
	};

	// class for multiplying two values - tested
	template <class U> class Multiply : public IArithmeticCombinable<U>
	{
		// member functions
	public:
		virtual U ApplyCombiner(const U& in1, const U& in2) const
		{
			return in1 * in2;
		}
		virtual U GetIdentityElement() const
		{
			U val;
			GenericType gen_val = val;
			return gen_val.GetMultiplicativeIdentityElement();
		}
		virtual std::string GetName() const
		{
			return "Multiply";
		}
	};

	// class for subtracting two values - tested
	template <class U> class Subtract : public Add<U>
	{
		// member functions
	public:
		virtual U ApplyCombiner(const U& in1, const U& in2) const
		{
			return in1 - in2;
		}
		virtual std::string GetName() const
		{
			return "Subtract";
		}
	};

	// class for dividing two values - tested
	template <class U> class Divide : public Multiply<U>
	{
		// member functions
	public:
		virtual U ApplyCombiner(const U& in1, const U& in2) const
		{
			return in1 / in2;
		}
		virtual std::string GetName() const
		{
			return "Divide";
		}
	};

	// arithmetic non-member operators

	template <class U> NArrayAxiomatic<U> operator +(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Add<U> add;
		return CombineValues(in1, in2, add);
	}

	template <class U> NArrayAxiomatic<U> operator -(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Subtract<U> subtract;
		return CombineValues(in1, in2, subtract);
	}

	template <class U> NArrayAxiomatic<U> operator *(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Multiply<U> multiply;
		return CombineValues(in1, in2, multiply);
	}

	template <class U> NArrayAxiomatic<U> operator /(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Divide<U> divide;
		return CombineValues(in1, in2, divide);
	}

	template <class T, class U> NArrayRecursive<T, U> operator +(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Add<U> add;
		return CombineValues(in1, in2, add);
	}

	template <class T, class U> NArrayRecursive<T, U> operator -(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Subtract<U> subtract;
		return CombineValues(in1, in2, subtract);
	}

	template <class T, class U> NArrayRecursive<T, U> operator *(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Multiply<U> multiply;
		return CombineValues(in1, in2, multiply);
	}

	template <class T, class U> NArrayRecursive<T, U> operator /(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Divide<U> divide;
		return CombineValues(in1, in2, divide);
	}
}
