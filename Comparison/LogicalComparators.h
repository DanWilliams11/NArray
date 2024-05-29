#pragma once

#include "../Core/NArrayDefinition.h"
#include "../GenericType/GenericType.h"
#include "IComparable.h"

namespace NArray
{
	// class for AND'ing two values - tested
	template <class U> class And : public ILogicalComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 && in2;
		}
		virtual bool GetIdentityElement() const
		{
			return GenericType::MultiplicativeIdentityElement(GenericType::Bool);
		}
		virtual std::string GetName() const
		{
			return "And";
		}
	};

	// class for NAND'ing two values - tested
	template <class U> class Nand : public And<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return !And::ApplyComparator(in1, in2);
		}
		virtual std::string GetName() const
		{
			return "Nand";
		}
	};

	// class for OR'ing two values - tested
	template <class U> class Or : public ILogicalComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 || in2;
		}
		virtual bool GetIdentityElement() const
		{
			return GenericType::AdditiveIdentityElement(GenericType::Bool);
		}
		virtual std::string GetName() const
		{
			return "Or";
		}
	};

	// class for NOR'ing two values - tested
	template <class U> class Nor : public Or<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return !Or::ApplyComparator(in1, in2);
		}
		virtual std::string GetName() const
		{
			return "Nor";
		}
	};

	// class for XOR'ing two values - tested
	template <class U> class Xor : public Or<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 != in2;
		}
		virtual std::string GetName() const
		{
			return "Xor";
		}
	};

	// logical non-member operators

	template <class U> bool operator ||(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Or<U> or;
		return CompareValues(in1, in2, or);
	}

	template <class U> bool operator &&(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		And<U> and;
		return CompareValues(in1, in2, and);
	}

	template <class T, class U> bool operator ||(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Or<U> or;
		return CompareValues(in1, in2, or);
	}

	template <class T, class U> bool operator &&(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		And<U> and;
		return CompareValues(in1, in2, and);
	}
}
