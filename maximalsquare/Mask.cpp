#include "maximalsquare.h"

namespace core
{
	void Mask::Create(Int32 lengths, Int32 nmembs)
	{
		auto	dv = core::div(lengths);
		if (dv.rem) ++dv.quot;

		this->wordwidths_ = dv.quot;
		this->bitswidths_ = this->wordwidths_ * __NBITMASK;
		this->resize(this->wordwidths_ * nmembs, 0);
	}

	void Mask::Initialize(Int32 cbSize)
	{
		this->resize(cbSize, 0);
	}

	size_t Mask::GetValue(Int32 pos)
	{
		if (pos >= (static_cast<Int32>(this->size()) * __NBITMASK) || pos < 0)
			return ~(0);

		return (*this)[static_cast<Int32>(pos / __NBITMASK)];
	}

	void Mask::SetEmpty()
	{
		std::fill(this->begin(), this->end(), 0);
	}

	void Mask::SetAt(Int32 pos, bool flags /* = true */)
	{
		if (pos >= (static_cast<Int32>(this->size()) * __NBITMASK) || pos < 0)
			return;

		auto	dv = core::div(pos);
		if (flags)
			(*this)[dv.quot] |= core::divmaskbits(dv);
		else
			(*this)[dv.quot] &= ~core::divmaskbits(dv);
	}

	void Mask::ClrAt(Int32 pos)
	{
		return Mask::SetAt(pos, false);
	}

	bool Mask::IsSet(Int32 pos) const
	{
		if (pos >= (static_cast<Int32>(this->size()) * __NBITMASK) || pos < 0)
			return true;

		auto	dv = core::div(pos);
		return ((*this)[dv.quot] & core::divmaskbits(dv)) ? true : false;
	}
};