#pragma once

#include "CompositionImplementation.h"
#include "../Useful/UsefulOperators.h"

namespace NArray
{
	// composition for making a square n-dimensional array the identity n-dimensional array - tested
	template <class S, class V> class SetToIdentity : public Composition<S, V>
	{
		// member functions
	public:
		SetToIdentity()
		{
			SetToZero<S, V>* set_to_zero = new SetToZero<S, V>;
			IsNotOnDiag<S, V>* is_not_on_diag = new IsNotOnDiag<S, V>;
			SetToOne<S, V>* set_to_one = new SetToOne<S, V>;
			IsOnDiag<S, V>* is_on_diag = new IsOnDiag<S, V>;
			ReComposition(4, set_to_zero, is_not_on_diag, set_to_one, is_on_diag);
		}
	};

	// composition for making a square n-dimensional array symmetric using its LTR or UTR - tested
	template <class S, class V> class SetToSymmetric : public Composition<S, V>
	{
		// member functions
	public:
		SetToSymmetric(NArraySupport::RegionSpecifier region_specifier)
		{
			if (region_specifier == NArraySupport::LTR)
			{
				CopyToTranspose<S, V>* copy_to_transpose = new CopyToTranspose<S, V>;
				IsBelowDiag<S, V>* is_below_diag = new IsBelowDiag<S, V>;
				ReComposition(2, copy_to_transpose, is_below_diag);
			}
			else if (region_specifier == NArraySupport::UTR)
			{
				CopyToTranspose<S, V>* copy_to_transpose = new CopyToTranspose<S, V>;
				IsAboveDiag<S, V>* is_above_diag = new IsAboveDiag<S, V>;
				ReComposition(2, copy_to_transpose, is_above_diag);
			}
			else
			{
				throw(std::invalid_argument("NArray: unrecognised region specifier in 'SetToSymmetric'\n"));
			}
		}
	};

	// composition for making a square n-dimensional array the transpose of itself - tested
	template <class S, class V> class SetToTranspose : public Composition<S, V>
	{
		// member functions
	public:
		SetToTranspose()
		{
			SwapWithTranspose<S, V>* swap_with_transpose = new SwapWithTranspose<S, V>;
			IsBelowDiag<S, V>* is_below_diag = new IsBelowDiag<S, V>;
			ReComposition(2, swap_with_transpose, is_below_diag);
		}
	};

	// composition for setting the LTR, UTR or DIAG of a square n-dimensional array to one (the multiplicative identity element) - tested
	template <class S, class V> class SetTriangleToOne : public Composition<S, V>
	{
		// member functions
	public:
		SetTriangleToOne(NArraySupport::RegionSpecifier region_specifier)
		{
			if (region_specifier == NArraySupport::LTR)
			{
				SetToOne<S, V>* set_to_one = new SetToOne<S, V>;
				IsBelowDiag<S, V>* is_below_diag = new IsBelowDiag<S, V>;
				ReComposition(2, set_to_one, is_below_diag);
			}
			else if (region_specifier == NArraySupport::UTR)
			{
				SetToOne<S, V>* set_to_one = new SetToOne<S, V>;
				IsAboveDiag<S, V>* is_above_diag = new IsAboveDiag<S, V>;
				ReComposition(2, set_to_one, is_above_diag);
			}
			else if (region_specifier == NArraySupport::DIAG)
			{
				SetToOne<S, V>* set_to_one = new SetToOne<S, V>;
				IsOnDiag<S, V>* is_on_diag = new IsOnDiag<S, V>;
				ReComposition(2, set_to_one, is_on_diag);
			}
			else
			{
				throw(std::invalid_argument("NArray: unrecognised region specifier in 'SetTriangleToOne'\n"));
			}
		}
	};

	// composition for setting the LTR, UTR or DIAG of a square n-dimensional array to the same value - tested
	template <class S, class V> class SetTriangleToValue : public Composition<S, V>
	{
		// member functions
	public:
		SetTriangleToValue(NArraySupport::RegionSpecifier region_specifier, V value)
		{
			if (region_specifier == NArraySupport::LTR)
			{
				SetToValue<S, V>* set_to_value = new SetToValue<S, V>(value);
				IsBelowDiag<S, V>* is_below_diag = new IsBelowDiag<S, V>;
				ReComposition(2, set_to_value, is_below_diag);
			}
			else if (region_specifier == NArraySupport::UTR)
			{
				SetToValue<S, V>* set_to_value = new SetToValue<S, V>(value);
				IsAboveDiag<S, V>* is_above_diag = new IsAboveDiag<S, V>;
				ReComposition(2, set_to_value, is_above_diag);
			}
			else if (region_specifier == NArraySupport::DIAG)
			{
				SetToValue<S, V>* set_to_value = new SetToValue<S, V>(value);
				IsOnDiag<S, V>* is_on_diag = new IsOnDiag<S, V>;
				ReComposition(2, set_to_value, is_on_diag);
			}
			else
			{
				throw(std::invalid_argument("NArray: unrecognised region specifier in 'SetTriangleToValue'\n"));
			}
		}
	};

	// composition for setting the LTR, UTR or DIAG of a square n-dimensional array to zero (the additive identity element) - tested
	template <class S, class V> class SetTriangleToZero : public Composition<S, V>
	{
		// member functions
	public:
		SetTriangleToZero(NArraySupport::RegionSpecifier region_specifier)
		{
			if (region_specifier == NArraySupport::LTR)
			{
				SetToZero<S, V>* set_to_zero = new SetToZero<S, V>;
				IsBelowDiag<S, V>* is_below_diag = new IsBelowDiag<S, V>;
				ReComposition(2, set_to_zero, is_below_diag);
			}
			else if (region_specifier == NArraySupport::UTR)
			{
				SetToZero<S, V>* set_to_zero = new SetToZero<S, V>;
				IsAboveDiag<S, V>* is_above_diag = new IsAboveDiag<S, V>;
				ReComposition(2, set_to_zero, is_above_diag);
			}
			else if (region_specifier == NArraySupport::DIAG)
			{
				SetToZero<S, V>* set_to_zero = new SetToZero<S, V>;
				IsOnDiag<S, V>* is_on_diag = new IsOnDiag<S, V>;
				ReComposition(2, set_to_zero, is_on_diag);
			}
			else
			{
				throw(std::invalid_argument("NArray: unrecognised region specifier in 'SetTriangleToZero'\n"));
			}
		}
	};
}
