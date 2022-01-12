#pragma once

namespace core
{
	const	Int32	__NBITMASK = static_cast<Int32>(8 * sizeof(size_t));

	inline auto div(Int32 pos) { return std::div(pos, __NBITMASK); }
	inline auto divmaskbits(const div_t& dv) { return (size_t)1 << dv.rem; }


	class Mask : public std::vector<size_t>
	{
		Int32	bitswidths_ = 0;
		Int32	wordwidths_ = 0;

	public:
		auto	GetBitsWidths() const { return bitswidths_; }
		auto	GetWordWidths() const { return wordwidths_; }
		//
		void	Create(Int32 lengths, Int32 nmembs);
		void	Initialize(Int32 cbSize);
		void	SetEmpty();
		//
		size_t	GetValue(Int32 pos);
		void	SetAt(Int32 pos, bool flags = true);
		void	ClrAt(Int32 pos);
		bool	IsSet(Int32 pos) const;
	};
};
