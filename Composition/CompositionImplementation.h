#pragma once

#include "CompositionDefinition.h"

using namespace NArray;

// member functions

template <class S, class V> Composition<S, V>::Composition()
{
	m_size = 0;
	m_composition = nullptr;
}

template <class S, class V> Composition<S, V>::Composition(int size, ...)
{
	typedef SingleThreadedOperator<S, V>* wibble; // have to do this because of problem with commas in macro expansion of va_arg

	m_size = size;
	m_composition = new wibble[size];
	va_list args;
	va_start(args, size);
	for (int i = 1; i <= size; i++)
	{
		wibble op = va_arg(args, wibble);
		PutCompositionAtReversePosition(i, op);
	}
	va_end(args);
}

template <class S, class V> Composition<S, V>::Composition(const Composition& c)
{
	m_size = c.m_size;
	m_composition = new SingleThreadedOperator<S, V>*[c.m_size];
	for (int i = 0; i < c.m_size; i++)
	{
		m_composition[i] = c.m_composition[i];
	}
}

template <class S, class V> Composition<S, V>::~Composition()
{
	DestroyConposition();
}

template <class S, class V> Composition<S, V>& Composition<S, V>::operator =(const Composition& c)
{
	if (m_size != c.m_size)
	{
		DestroyConposition();
		m_size = c.m_size;
		m_composition = new SingleThreadedOperator<S, V>*[c.m_size];
	}
	for (int i = 0; i < c.m_size; i++)
	{
		m_composition[i] = c.m_composition[i];
	}
	return *this;
}

template <class S, class V> bool Composition<S, V>::ApplyOperator(const Index& index, V& value, const S& in, S& out)
{
	for (int i = 1; i <= m_size; i++)
	{
		SingleThreadedOperator<S, V>* op = GetCompositionAtPosition(i);
		bool val = op->ApplyOperator(index, value, in, out);
		if (!val) // an operator has returned false so skip the next operator in the composition
			i++;
	}
	return true;
}

template <class S, class V> void Composition<S, V>::DestroyConposition()
{
	if (m_composition)
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_composition[i])
			{
				delete m_composition[i];
				m_composition[i] = nullptr;
			}
		}
		delete[] m_composition;
		m_composition = nullptr;
	}
}

// uses position to get the operator for that position
template <class S, class V> SingleThreadedOperator<S, V>* Composition<S, V>::GetCompositionAtPosition(int position) const
{
	if (position > 0 && position <= m_size)
		return m_composition[position - 1]; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetCompositionAtPosition'\n"));
}

template <class S, class V> int Composition<S, V>::GetSize() const
{
	return m_size;
}

// uses position to put the operator for the reverse of that position
template <class S, class V> void Composition<S, V>::PutCompositionAtReversePosition(int position, SingleThreadedOperator<S, V>* op)
{
	int pos = m_size - position; // the last position is the first operator to be applied
	if (pos >= 0 && pos < m_size)
		m_composition[pos] = op;
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutCompositionAtReversePosition'\n"));
}

template <class S, class V> void Composition<S, V>::ReComposition(int size, ...)
{
	typedef SingleThreadedOperator<S, V>* wibble; // have to do this because of problem with commas in macro expansion of va_arg

	if (m_size != size)
	{
		DestroyConposition();
		m_size = size;
		m_composition = new wibble[size];
	}
	va_list args;
	va_start(args, size);
	for (int i = 1; i <= size; i++)
	{
		wibble op = va_arg(args, wibble);
		PutCompositionAtReversePosition(i, op);
	}
	va_end(args);
}
