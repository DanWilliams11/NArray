#pragma once

#include "../Core/NArrayDefinition.h"
#include "ComparisonFunctions.h"
#include "IComparable.h"

namespace NArray
{
	// class for determining if two values are equal - tested
	template <class U> class Equal : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 == in2;
		}
		virtual std::string GetName() const
		{
			return "Equal";
		}
	};

	// class for determining if the first value is greater than the second value - tested
	template <class U> class GreaterThan : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 > in2;
		}
		virtual std::string GetName() const
		{
			return "GreaterThan";
		}
	};

	// class for determining if the first value is greater than or equal to the second value - tested
	template <class U> class GreaterThanOrEqual : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 >= in2;
		}
		virtual std::string GetName() const
		{
			return "GreaterThanOrEqual";
		}
	};

	// class for determining if the first value is less than the second value - tested
	template <class U> class LessThan : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 < in2;
		}
		virtual std::string GetName() const
		{
			return "LessThan";
		}
	};

	// class for determining if the first value is less than or equal to the second value - tested
	template <class U> class LessThanOrEqual : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 <= in2;
		}
		virtual std::string GetName() const
		{
			return "LessThanOrEqual";
		}
	};

	// class for determining if two values are not equal - tested
	template <class U> class NotEqual : public IComparable<U>
	{
		// member functions
	public:
		virtual bool ApplyComparator(const U& in1, const U& in2) const
		{
			return in1 != in2;
		}
		virtual std::string GetName() const
		{
			return "NotEqual";
		}
	};

	// relational non-member operators

	template <class U> bool operator ==(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		Equal<U> equal;
		return CompareValues(in1, in2, equal);
	}

	template <class U> bool operator !=(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		NotEqual<U> not_equal;
		return CompareValues(in1, in2, not_equal);
	}

	template <class U> bool operator <(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		LessThan<U> less_than;
		return CompareValues(in1, in2, less_than);
	}

	template <class U> bool operator <=(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		LessThanOrEqual<U> less_than_or_equal;
		return CompareValues(in1, in2, less_than_or_equal);
	}

	template <class U> bool operator >(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		GreaterThan<U> greater_than;
		return CompareValues(in1, in2, greater_than);
	}

	template <class U> bool operator >=(NArrayAxiomatic<U>& in1, NArrayAxiomatic<U>& in2)
	{
		GreaterThanOrEqual<U> greater_than_or_equal;
		return CompareValues(in1, in2, greater_than_or_equal);
	}

	template <class T, class U> bool operator ==(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		Equal<U> equal;
		return CompareValues(in1, in2, equal);
	}

	template <class T, class U> bool operator !=(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		NotEqual<U> not_equal;
		return CompareValues(in1, in2, not_equal);
	}

	template <class T, class U> bool operator <(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		LessThan<U> less_than;
		return CompareValues(in1, in2, less_than);
	}

	template <class T, class U> bool operator <=(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		LessThanOrEqual<U> less_than_or_equal;
		return CompareValues(in1, in2, less_than_or_equal);
	}

	template <class T, class U> bool operator >(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		GreaterThan<U> greater_than;
		return CompareValues(in1, in2, greater_than);
	}

	template <class T, class U> bool operator >=(NArrayRecursive<T, U>& in1, NArrayRecursive<T, U>& in2)
	{
		GreaterThanOrEqual<U> greater_than_or_equal;
		return CompareValues(in1, in2, greater_than_or_equal);
	}
}
