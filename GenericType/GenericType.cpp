#include "GenericType.h"

using namespace NArray;

// member functions

// default constructor

GenericType::GenericType()
{
	m_type = Void;
	m_value = nullptr;
}

// constructor for storing pointer types

GenericType::GenericType(void* value)
{
	m_type = Void;
	m_value = value;
}

// constructors for storing built-in types

GenericType::GenericType(const bool& value)
{
	m_type = Bool;
	m_value = new bool(value);
}

GenericType::GenericType(const char& value)
{
	m_type = Char;
	m_value = new char(value);
}

GenericType::GenericType(const int& value)
{
	m_type = Int;
	m_value = new int(value);
}

GenericType::GenericType(const size_t& value)
{
	m_type = SizeT;
	m_value = new size_t(value);
}

GenericType::GenericType(const double& value)
{
	m_type = Double;
	m_value = new double(value);
}

// TODO add constructors for storing non built-in types - std::string and std::complex<double> have been added

GenericType::GenericType(const std::string& value)
{
	m_type = String;
	m_value = new std::string(value);
}

GenericType::GenericType(const std::complex<double>& value)
{
	m_type = ComplexDouble;
	m_value = new std::complex<double>(value);
}

// copy constructor

GenericType::GenericType(const GenericType& g)
{
	CreateGenericType(g);
}

// destructor

GenericType::~GenericType()
{
	DestroyGenericType();
}

// assignment operator

GenericType& GenericType::operator =(const GenericType& g)
{
	DestroyGenericType();
	CreateGenericType(g);
	return *this;
}

// cast operator for pointer types

GenericType::operator void*() const
{
	if (m_type == Void)
		return m_value;
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is void*\n"));
}

// cast operators for built-in types

GenericType::operator bool&() const
{
	if (m_type == Bool)
		return *((bool*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is bool\n"));
}

GenericType::operator char&() const
{
	if (m_type == Char)
		return *((char*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is char\n"));
}

GenericType::operator int&() const
{
	if (m_type == Int)
		return *((int*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is int\n"));
}

GenericType::operator size_t&() const
{
	if (m_type == SizeT)
		return *((size_t*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is size_t\n"));
}

GenericType::operator double&() const
{
	if (m_type == Double)
		return *((double*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is double\n"));
}

// TODO add cast operators for non built-in types - std::string and std::complex<double> have been added

GenericType::operator std::string&() const
{
	if (m_type == String)
		return *((std::string*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is std::string\n"));
}

GenericType::operator std::complex<double>&() const
{
	if (m_type == ComplexDouble)
		return *((std::complex<double>*) m_value);
	else
		throw(std::invalid_argument("NArray: Illegal cast - underlying type is std::complex<double>\n"));
}

// relational operators

bool GenericType::operator ==(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal relational operation - underlying types are different\n"));

	bool is_equal = false;
	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		if (lhs == rhs)
			is_equal = true;
	}
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (m_type == String)
	{
		std::string lhs = (std::string) *this;
		std::string rhs = (std::string) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else if (m_type == ComplexDouble)
	{
		std::complex<double> lhs = (std::complex<double>) *this;
		std::complex<double> rhs = (std::complex<double>) g;
		if (lhs == rhs)
			is_equal = true;
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in relational operator == or !=\n"));
	return is_equal;
}

bool GenericType::operator !=(const GenericType& g) const
{
	return !(*this == g);
}

bool GenericType::operator <(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal relational operation - underlying types are different\n"));

	bool is_less_than = false;
	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	else if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	// TODO add non built-in types - std::string has been added
	else if (m_type == String)
	{
		std::string lhs = (std::string) *this;
		std::string rhs = (std::string) g;
		if (lhs < rhs)
			is_less_than = true;
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in relational operator < or <=\n"));
	return is_less_than;
}

bool GenericType::operator <=(const GenericType& g) const
{
	return (*this < g) || (*this == g);
}

bool GenericType::operator >(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal relational operation - underlying types are different\n"));

	bool is_greater_than = false;
	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	else if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	// TODO add non built-in types - std::string has been added
	else if (m_type == String)
	{
		std::string lhs = (std::string) *this;
		std::string rhs = (std::string) g;
		if (lhs > rhs)
			is_greater_than = true;
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in relational operator > or >=\n"));
	return is_greater_than;
}

bool GenericType::operator >=(const GenericType& g) const
{
	return (*this > g) || (*this == g);
}

// arithmetic operators

GenericType GenericType::operator +(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal arithmetic operation - underlying types are different\n"));

	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		return GenericType(lhs || rhs);
	}
	else if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		return GenericType(lhs + rhs);
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		return GenericType(lhs + rhs);
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		return GenericType(lhs + rhs);
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		return GenericType(lhs + rhs);
	}
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (m_type == String)
	{
		std::string lhs = (std::string) *this;
		std::string rhs = (std::string) g;
		return GenericType(lhs + rhs);
	}
	else if (m_type == ComplexDouble)
	{
		std::complex<double> lhs = (std::complex<double>) *this;
		std::complex<double> rhs = (std::complex<double>) g;
		return GenericType(lhs + rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in arithmetic operator + or +=\n"));
}

GenericType GenericType::operator +=(const GenericType& g)
{
	*this = *this + g;
	return *this;
}

GenericType GenericType::operator -(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal arithmetic operation - underlying types are different\n"));

	if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		return GenericType(lhs - rhs);
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		return GenericType(lhs - rhs);
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		return GenericType(lhs - rhs);
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		return GenericType(lhs - rhs);
	}
	// TODO add non built-in types - std::complex<double> has been added
	else if (m_type == ComplexDouble)
	{
		std::complex<double> lhs = (std::complex<double>) *this;
		std::complex<double> rhs = (std::complex<double>) g;
		return GenericType(lhs - rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in arithmetic operator - or -=\n"));
}

GenericType GenericType::operator -=(const GenericType& g)
{
	*this = *this - g;
	return *this;
}

GenericType GenericType::operator *(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal arithmetic operation - underlying types are different\n"));

	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		return GenericType(lhs && rhs);
	}
	else if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		return GenericType(lhs * rhs);
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		return GenericType(lhs * rhs);
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		return GenericType(lhs * rhs);
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		return GenericType(lhs * rhs);
	}
	// TODO add non built-in types - std::complex<double> has been added
	else if (m_type == ComplexDouble)
	{
		std::complex<double> lhs = (std::complex<double>) *this;
		std::complex<double> rhs = (std::complex<double>) g;
		return GenericType(lhs * rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in arithmetic operator * or *=\n"));
}

GenericType GenericType::operator *=(const GenericType& g)
{
	*this = *this * g;
	return *this;
}

GenericType GenericType::operator /(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal arithmetic operation - underlying types are different\n"));

	if (m_type == Char)
	{
		char lhs = (char) *this;
		char rhs = (char) g;
		return GenericType(lhs / rhs);
	}
	else if (m_type == Int)
	{
		int lhs = (int) *this;
		int rhs = (int) g;
		return GenericType(lhs / rhs);
	}
	else if (m_type == SizeT)
	{
		size_t lhs = (size_t) *this;
		size_t rhs = (size_t) g;
		return GenericType(lhs / rhs);
	}
	else if (m_type == Double)
	{
		double lhs = (double) *this;
		double rhs = (double) g;
		return GenericType(lhs / rhs);
	}
	// TODO add non built-in types - std::complex<double> has been added
	else if (m_type == ComplexDouble)
	{
		std::complex<double> lhs = (std::complex<double>) *this;
		std::complex<double> rhs = (std::complex<double>) g;
		return GenericType(lhs / rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in arithmetic operator / or /=\n"));
}

GenericType GenericType::operator /=(const GenericType& g)
{
	*this = *this / g;
	return *this;
}

// logical operators

GenericType GenericType::operator ||(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal logical operation - underlying types are different\n"));

	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		return GenericType(lhs || rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in logical operator ||\n"));
}

GenericType GenericType::operator &&(const GenericType& g) const
{
	if (m_type != g.GetType())
		throw(std::invalid_argument("NArray: Illegal logical operation - underlying types are different\n"));

	if (m_type == Bool)
	{
		bool lhs = (bool) *this;
		bool rhs = (bool) g;
		return GenericType(lhs && rhs);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in logical operator &&\n"));
}

// other member functions

GenericType GenericType::AdditiveIdentityElement(GenericTypeId type)
{
	if (type == Bool)
	{
		bool additive_identity_element = false;
		return GenericType(additive_identity_element);
	}
	else if (type == Char)
	{
		char additive_identity_element = '0';
		return GenericType(additive_identity_element);
	}
	else if (type == Int)
	{
		int additive_identity_element = 0;
		return GenericType(additive_identity_element);
	}
	else if (type == SizeT)
	{
		size_t additive_identity_element = 0;
		return GenericType(additive_identity_element);
	}
	else if (type == Double)
	{
		double additive_identity_element = 0.0;
		return GenericType(additive_identity_element);
	}
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (type == String)
	{
		std::string additive_identity_element = "zero";
		return GenericType(additive_identity_element);
	}
	else if (type == ComplexDouble)
	{
		std::complex<double> additive_identity_element(0.0, 0.0);
		return GenericType(additive_identity_element);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in 'AdditiveIdentityElement'\n"));
}

void GenericType::CreateGenericType(const GenericType& g)
{
	m_type = g.GetType();
	if (m_type == Void)
		m_value = g.m_value;
	else if (m_type == Bool)
		m_value = new bool((bool&) g);
	else if (m_type == Char)
		m_value = new char((char&) g);
	else if (m_type == Int)
		m_value = new int((int&) g);
	else if (m_type == SizeT)
		m_value = new size_t((size_t&) g);
	else if (m_type == Double)
		m_value = new double((double&) g);
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (m_type == String)
		m_value = new std::string((std::string&) g);
	else if (m_type == ComplexDouble)
		m_value = new std::complex<double>((std::complex<double>&) g);
	else
		throw(std::invalid_argument("NArray: Unsupported type in 'GenericType' constructor\n"));
}

GenericType GenericType::DefaultValue(GenericTypeId type)
{
	if (type == Void)
		return GenericType();
	else if (type == Bool)
		return AdditiveIdentityElement(type);
	else if (type == Char)
		return GenericType('a');
	else if (type == Int)
		return AdditiveIdentityElement(type);
	else if (type == SizeT)
		return AdditiveIdentityElement(type);
	else if (type == Double)
		return AdditiveIdentityElement(type);
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (type == String)
		return AdditiveIdentityElement(type);
	else if (type == ComplexDouble)
		return AdditiveIdentityElement(type);
	else
		throw(std::invalid_argument("NArray: Unsupported type in 'DefaultValue'\n"));
}

void GenericType::DestroyGenericType()
{
	if (m_type == Bool && m_value)
		delete &((bool&) *this);
	else if (m_type == Char && m_value)
		delete &((char&) *this);
	else if (m_type == Int && m_value)
		delete &((int&) *this);
	else if (m_type == SizeT && m_value)
		delete &((size_t&) *this);
	else if (m_type == Double && m_value)
		delete &((double&) *this);
	// TODO add non built-in types - std::string and std::complex<double> have been added
	else if (m_type == String && m_value)
		delete &((std::string&) *this);
	else if (m_type == ComplexDouble && m_value)
		delete &((std::complex<double>&) *this);
	m_type = Void;
	m_value = nullptr;
	// in the case of pointer types, the lifetimes of the objects pointed to are not controlled by this class
	// the objects are to be created and destroyed by the client code
	// no else as shouldn't throw an exception from within a destructor
	// TODO flag error another way?
}

GenericType GenericType::GetAdditiveIdentityElement() const
{
	return AdditiveIdentityElement(m_type);
}

GenericType GenericType::GetDefaultValue() const
{
	return DefaultValue(m_type);
}

GenericType GenericType::GetMultiplicativeIdentityElement() const
{
	return MultiplicativeIdentityElement(m_type);
}

GenericType::GenericTypeId GenericType::GetType() const
{
	return m_type;
}

GenericType GenericType::MultiplicativeIdentityElement(GenericType::GenericTypeId type)
{
	if (type == Bool)
	{
		bool multiplicative_identity_element = true;
		return GenericType(multiplicative_identity_element);
	}
	else if (type == Char)
	{
		char multiplicative_identity_element = '1';
		return GenericType(multiplicative_identity_element);
	}
	else if (type == Int)
	{
		int multiplicative_identity_element = 1;
		return GenericType(multiplicative_identity_element);
	}
	else if (type == SizeT)
	{
		size_t multiplicative_identity_element = 1;
		return GenericType(multiplicative_identity_element);
	}
	else if (type == Double)
	{
		double multiplicative_identity_element = 1.0;
		return GenericType(multiplicative_identity_element);
	}
	// TODO add non built-in types - std::complex<double> has been added
	else if (type == String)
	{
		std::string multiplicative_identity_element = "one";
		return GenericType(multiplicative_identity_element);
	}
	else if (type == ComplexDouble)
	{
		std::complex<double> multiplicative_identity_element(1.0, 1.0);
		return GenericType(multiplicative_identity_element);
	}
	else
		throw(std::invalid_argument("NArray: Unsupported type in 'MultiplicativeIdentityElement'\n"));
}

void GenericType::Read(std::istream& input_stream)
{
	if (input_stream.good())
	{
		std::string ptr;
		if (m_type == Void)
			input_stream >> ptr;
		else if (m_type == Bool)
			input_stream >> ((bool&)*this);
		else if (m_type == Char)
			input_stream >> ((char&)*this);
		else if (m_type == Int)
			input_stream >> ((int&)*this);
		else if (m_type == SizeT)
			input_stream >> ((size_t&)*this);
		else if (m_type == Double)
			input_stream >> ((double&)*this);
		// TODO add non built-in types - std::string and std::complex<double> have been added
		else if (m_type == String)
			input_stream >> ((std::string&) *this);
		else if (m_type == ComplexDouble)
			input_stream >> ((std::complex<double>&) *this);
		else
			throw(std::invalid_argument("NArray: Unsupported type in 'GenericType::Read'\n"));
	}
	else
		throw std::ios_base::failure("NArray: Input stream failed in 'GenericType::Read'\n");
}

void GenericType::Write(std::ostream& output_stream) const
{
	if (output_stream.good())
	{
		if (m_type == Void)
			output_stream << "PTR" << ' ';
		else if (m_type == Bool)
		{
			std::string str = "false";
			if ((bool&) *this == true)
				str = "true";
			output_stream << str << ' ';
		}
		else if (m_type == Char)
			output_stream << ((char&) *this) << ' ';
		else if (m_type == Int)
			output_stream << ((int&) *this) << ' ';
		else if (m_type == SizeT)
			output_stream << ((size_t&) *this) << ' ';
		else if (m_type == Double)
			output_stream << ((double&) *this) << ' ';
		// TODO add non built-in types - std::string and std::complex<double> have been added
		else if (m_type == String)
			output_stream << ((std::string&) *this) << ' ';
		else if (m_type == ComplexDouble)
			output_stream << ((std::complex<double>&) *this) << ' ';
		else
			throw(std::invalid_argument("NArray: Unsupported type in 'GenericType::Write'\n"));
	}
	else
		throw std::ios_base::failure("NArray: Output stream failed in 'GenericType::Write'\n");
}

// non-member operator implementations

// serialisation operators

std::ostream& NArray::operator <<(std::ostream& output_stream, const GenericType& in)
{
	in.Write(output_stream);
	return output_stream;
}

std::istream& NArray::operator >>(std::istream& input_stream, GenericType& in)
{
	in.Read(input_stream);
	return input_stream;
}
