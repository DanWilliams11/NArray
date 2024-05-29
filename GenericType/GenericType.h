#pragma once

#include <iostream>

// TODO #include non built-in types - <string> and <complex> have been added
#include <string>
#include <complex>

/*
Type-safe wrapper for storing arbitrary types (wraps void*).
Allows arrays of mixed type.
The type can change at run-time.
Note:
In the case of pointer types, the lifetimes of the objects pointed to are not controlled by this class.
These objects are to be created and destroyed by the client code.
You cannot template this class - it would stop it working with arrays of mixed type.
Given this, you cannot function template its constructors either.
*/

namespace NArray
{
	// classes
	// class for storing arbitrary types
	class GenericType
	{
		// member variables
	public:
		enum GenericTypeId
		{
			Void,			// for pointer types
			Bool,			// for built-in bools
			Char,			// for built-in chars
			Int,			// for built-in ints
			SizeT,			// for built-in unsigned ints and unsigned int64s
			Double,			// for built-in doubles
			// TODO add enums for non built-in types - String (for std::string) and ComplexDouble (for std::complex<double>) have been added
			String,			// for std::string
			ComplexDouble,	// for std::complex<double>
		};
	private:
		GenericTypeId m_type;
		void* m_value;
		// member functions
	private:
				void CreateGenericType(const GenericType& g);
				void DestroyGenericType();
	public:
				// default constructor
				GenericType();
				// constructor for storing pointer types
				GenericType(void* value);
				// constructors for storing built-in types
				GenericType(const bool& value);
				GenericType(const char& value);
				GenericType(const int& value);
				GenericType(const size_t& value);
				GenericType(const double& value);
				// TODO declare constructors for storing non built-in types - std::string and std::complex<double> have been added
				GenericType(const std::string& value);
				GenericType(const std::complex<double>& value);
				// copy constructor
				GenericType(const GenericType& g);
				// destructor
		virtual ~GenericType();
				// assignment operator
				GenericType& operator =(const GenericType& g);
				// cast operator for pointer types
				operator void*() const;
				// cast operators for built-in types
				operator bool&() const;
				operator char&() const;
				operator int&() const;
				operator size_t&() const;
				operator double&() const;
				// TODO declare cast operators for non built-in types - std::string and std::complex<double> have been added
				operator std::string&() const;
				operator std::complex<double>&() const;
				// relational operators
				bool operator ==(const GenericType& g) const;
				bool operator !=(const GenericType& g) const;
				bool operator <(const GenericType& g) const;
				bool operator <=(const GenericType& g) const;
				bool operator >(const GenericType& g) const;
				bool operator >=(const GenericType& g) const;
				// arithmetic operators
				GenericType operator +(const GenericType& g) const;
				GenericType operator +=(const GenericType& g);
				GenericType operator -(const GenericType& g) const;
				GenericType operator -=(const GenericType& g);
				GenericType operator *(const GenericType& g) const;
				GenericType operator *=(const GenericType& g);
				GenericType operator /(const GenericType& g) const;
				GenericType operator /=(const GenericType& g);
				// logical operators
				GenericType operator ||(const GenericType& g) const;
				GenericType operator &&(const GenericType& g) const;
				// other member functions
				static GenericType AdditiveIdentityElement(GenericTypeId type);
				static GenericType DefaultValue(GenericTypeId type);
				GenericType GetAdditiveIdentityElement() const;
				GenericType GetDefaultValue() const;
				GenericType GetMultiplicativeIdentityElement() const;
				GenericTypeId GetType() const;
				static GenericType MultiplicativeIdentityElement(GenericTypeId type);
				void Read(std::istream& input_stream);
				// void Serialise(KArchive& ka);
				void Write(std::ostream& output_stream) const;
	};

	// non-member operator declarations
	// serialisation operators
	std::ostream& operator <<(std::ostream& output_stream, const GenericType& in);
	std::istream& operator >>(std::istream& input_stream, GenericType& in);
}
