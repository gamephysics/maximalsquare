#include "maximalsquare.h"

namespace core
{
	const Int64 FieldCollision::npos = ~(0);

	void FieldCollision::InitInstance(const Int32& width, const Int32& height)
	{
		std::lock_guard<std::recursive_mutex>	lock(lock_);

		this->width_ = width;
		this->height_ = height;

		xBoundaryPoints.Create(width, height);
		yBoundaryPoints.Create(height, width);

		// OUT OF BOUND COLLISION SET
		for (int x = width; x < xBoundaryPoints.GetBitsWidths(); ++x)
		{
			for (int y = height; y < yBoundaryPoints.GetBitsWidths(); ++y)
			{
				SetAt(x, y);
			}
		}
	}

	void FieldCollision::CopyToFieldCollision(FieldCollision::SharedPtr newCollision)
	{
		if (!newCollision)
			return;

		CopyToFieldCollision(newCollision.get());
	}

	void FieldCollision::CopyToFieldCollision(FieldCollision*			newCollision)
	{
		if (!newCollision)
			return;

		std::lock_guard<std::recursive_mutex>	lock(lock_);

		newCollision->xBoundaryPoints = this->xBoundaryPoints;
		newCollision->yBoundaryPoints = this->yBoundaryPoints;

		newCollision->SetWidth(width_);
		newCollision->SetHeight(height_);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//	[충돌맵]
	void FieldCollision::SetAt(Int32 x, Int32 y)
	{
		std::lock_guard<std::recursive_mutex>	lock(lock_);

		xBoundaryPoints.SetAt(GetPosX(x, y));
		yBoundaryPoints.SetAt(GetPosY(x, y));
	}

	void FieldCollision::ClrAt(Int32 x, Int32 y)
	{
		std::lock_guard<std::recursive_mutex>	lock(lock_);

		xBoundaryPoints.ClrAt(GetPosX(x, y));
		yBoundaryPoints.ClrAt(GetPosY(x, y));
	}

	void FieldCollision::SetRange(Int32 xMin, Int32 yMin, Int32 xMax, Int32 yMax, bool flags)
	{
		std::lock_guard<std::recursive_mutex>	lock(lock_);

		for (Int32 x = xMin; x <= xMax; ++x)
		{
			for (Int32 y = yMin; y <= yMax; ++y)
			{
				if (flags)
				{
					xBoundaryPoints.SetAt(GetPosX(x, y));
					yBoundaryPoints.SetAt(GetPosY(x, y));
				}
				else
				{
					xBoundaryPoints.ClrAt(GetPosX(x, y));
					yBoundaryPoints.ClrAt(GetPosY(x, y));
				}
			}
		}
	}

	void FieldCollision::SetHorizon(Int32 x1, Int32 x2, Int32 y, bool flags)
	{
		for (int x = x1; x <= x2; ++x)
		{
			if (flags)	xBoundaryPoints.SetAt(GetPosX(x, y));
			else		xBoundaryPoints.ClrAt(GetPosX(x, y));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//	[객체충돌]
	bool FieldCollision::Insert(Int32 x, Int32 y, Int32 volume)
	{
		if (volume <= 0)
			return false;

		return Insert(x, y, volume, volume);
	}

	bool FieldCollision::Insert(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume)
	{
		if (xVolume <= 0 || yVolume <= 0)
			return false;

		Int32	xSize = xVolume;
		Int32	ySize = yVolume;

		if (x < 0) { xSize += x; x = 0; }
		if (y < 0) { ySize += y; y = 0; }
		if ((x + xSize) >= GetWidth())	xSize = (GetWidth() - x);
		if ((y + ySize) >= GetHeight())	ySize = (GetHeight() - y);

		for (Int32 cy = y; cy < (y + ySize); ++cy)
			for (Int32 cx = x; cx < (x + xSize); ++cx)
				FieldCollision::SetAt(cx, cy);

		return true;
	}

	bool FieldCollision::Delete(Int32 x, Int32 y, Int32 volume)
	{
		if (volume <= 0)
			return false;

		return Delete(x, y, volume, volume);
	}

	bool FieldCollision::Delete(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume)
	{
		if (xVolume <= 0 || yVolume <= 0)
			return false;

		Int32	xSize = xVolume;
		Int32	ySize = yVolume;

		if (x < 0) { xSize += x; x = 0; }
		if (y < 0) { ySize += y; y = 0; }
		if ((x + xSize) >= GetWidth())	xSize = (GetWidth() - x);
		if ((y + ySize) >= GetHeight())	ySize = (GetHeight() - y);

		for (Int32 cy = y; cy < (y + ySize); ++cy)
			for (Int32 cx = x; cx < (x + xSize); ++cx)
				FieldCollision::ClrAt(cx, cy);

		return true;
	}

	bool FieldCollision::Delete(Int32 x, Int32 y)
	{
		FieldCollision::ClrAt(x, y);
		return true;
	}

	bool FieldCollision::IsSpace(Int32 x, Int32 y, Int32 volume, const std::vector<core::Rect>* lstExceptRects /*= nullptr*/) const
	{
		if (lstExceptRects)
		{
			core::Rect rect;
			rect.SetRectVolume(x, y, volume, volume);

			for (auto& rcExcept : (*lstExceptRects))
			{
				if (rcExcept.IsIntersectRect(rect))
					return false;
			}
		}

		return IsEmpty(x, y, volume, volume);
	}

	bool FieldCollision::IsSpace(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume, const core::Rect* prcExcept /*= nullptr*/) const
	{
		if (prcExcept)
		{
			core::Rect rect;
			rect.SetRectVolume(x, y, xVolume, yVolume);

			if (prcExcept->IsIntersectRect(rect))
				return false;
		}

		return IsEmpty(x, y, xVolume, yVolume);
	}

	//	[일반함수]
	bool FieldCollision::IsCollision(Int32 x, Int32 y)
	{
		return xBoundaryPoints.IsSet(GetPosX(x, y));
	}

	bool FieldCollision::IsEmpty(Int32 x, Int32 y, Int32 xSize, Int32 ySize) const
	{
		if (x < 0 || y < 0) return false;
		if ((x + xSize) > GetWidth() || (y + ySize) > GetHeight())	return false;

		for (Int32 cy = 0; cy < ySize; ++cy)
		{
			for (Int32 cx = 0; cx < xSize; ++cx)
			{
				if (xBoundaryPoints.IsSet(GetPosX(x + cx, y+ cy)))
					return false;
			}
		}
		return true;
	}

	void FieldCollision::Dump()
	{
		for (Int32 cy = 0; cy < height_; ++cy)
		{
			for (Int32 cx = 0; cx < width_; ++cx)
			{
				std::printf("%d", this->IsCollision(cx, cy) ? 1 : 0);
			}
			std::printf("\n");
		}
	}

	Int32 FieldCollision::GetPosX(Int32 x, Int32 y) const
	{
		if (x < 0 || x >= xBoundaryPoints.GetBitsWidths() || y < 0 || y >= yBoundaryPoints.GetBitsWidths()) return npos;
		return x + y * xBoundaryPoints.GetBitsWidths();
	}

	Int32 FieldCollision::GetPosY(Int32 x, Int32 y) const
	{
		if (x < 0 || x >= xBoundaryPoints.GetBitsWidths() || y < 0 || y >= yBoundaryPoints.GetBitsWidths()) return npos;
		return x * yBoundaryPoints.GetBitsWidths() + y;
	}

	size_t FieldCollision::GetValueX(Int32 x, Int32 y)
	{
		return xBoundaryPoints.GetValue(GetPosX(x, y));
	}

	size_t FieldCollision::GetValueY(Int32 x, Int32 y)
	{
		return yBoundaryPoints.GetValue(GetPosY(x, y));
	}


	//maximal-square
	//Approach #3 (Better Dynamic Programming) [Accepted]
	//https://leetcode.com/problems/maximal-square/solution/
	
	thread_local std::vector<Int16>	FieldCollision::dpp_;

	std::vector<core::Point> FieldCollision::MaximalSquarePoints(const core::Rect& rect, Int32 n, const std::vector<core::Rect>* lstExceptRects, FieldCollision* pColl)
	{
		if (dpp_.size() <= 0) dpp_.resize(this->width_ + 1);

		// clear
		dpp_[rect.left]		= 0;
		dpp_[rect.left + 1] = 0;

		Int16 maxsqlen = 0, prev = 0;

		core::Rect LoopRect = rect;
		++LoopRect.left;
		++LoopRect.bottom;
		++LoopRect.right;
		++LoopRect.top;

		bool loopbreak = false;
		std::vector<core::Point>	candiates;

		for (Int32 i = LoopRect.bottom; i <= LoopRect.top && loopbreak == false; ++i)
		{
			for (Int32 j = LoopRect.left; j <= LoopRect.right && loopbreak == false; ++j)
			{
				if (i == LoopRect.bottom)	dpp_[j] = 0;
				Int16 temp = dpp_[j];

				// 충돌 체크
				bool isCollision = false;
				if (!isCollision)
					isCollision |= IsCollision(j - 1, i - 1);

				if (!isCollision)
					isCollision |= (pColl && pColl->IsCollision(j - 1, i - 1));

				if (!isCollision)
				{
					if (lstExceptRects)
					{
						for (auto& rect : (*lstExceptRects))
						{
							isCollision |= rect.PtInRect(j - 1, i - 1);
							if (isCollision)
								break;
						}
					}
				}

				// 충돌 없음/있음 따른 분기
				if (!isCollision)
				{
					dpp_[j] = std::min(std::min(dpp_[j - 1], prev), dpp_[j]) + 1;

					if (maxsqlen < dpp_[j] && n <= dpp_[j])
					{
						maxsqlen = dpp_[j];
						//posX = (j - 1);
						//posY = (i - 1); // 0 ~
						candiates.clear();
						candiates.emplace_back(core::Point((j - 1), (i - 1)));
					}
					else if (maxsqlen == dpp_[j] && n <= dpp_[j])
					{
						candiates.emplace_back(core::Point((j - 1), (i - 1)));
					}
				}
				else
				{
					dpp_[j] = 0;
				}
				prev = temp;

				if (maxsqlen >= n * 4 && candiates.size() > 16)	loopbreak = true;
			}
		}

		if (maxsqlen > (n * 2))
		{
			for (auto& point : candiates)
			{
				point.x -= (maxsqlen / 2);
				point.y -= (maxsqlen / 2);
			}
		}
		return candiates;
	}
};

