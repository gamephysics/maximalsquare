#pragma once


class COccupyCollision : public core::Rect	//	상속받아서 Boundary 처리
{
public:
	core::FieldCollision m_OccupyCollision;

	inline void InitInstance(Int32 LengthOfOneSide)
	{
		core::Rect::SetRect(0, 0, LengthOfOneSide - 1, LengthOfOneSide - 1);
		m_OccupyCollision.InitInstance(LengthOfOneSide, LengthOfOneSide);
	}


	inline bool	Insert(Int32 x, Int32 y, Int32 volume)
	{
		assert(core::Rect::PtInRect(x, y, volume) == true);
		if (core::Rect::PtInRect(x, y, volume) == false) return false;

		assert(volume > 0);
		if (volume <= 0) return false;

		if (IsSpace(x, y, volume) == true)
		{
			m_OccupyCollision.Insert(x, y, volume);
			return true;
		}
		return false;
	}

	inline bool	Delete(Int32 x, Int32 y, Int32 volume)
	{
		assert(core::Rect::PtInRect(x, y, volume) == true);
		if (core::Rect::PtInRect(x, y, volume) == false) return false;

		assert(volume > 0);
		if (volume <= 0) return false;

		if (IsSpace(x, y, volume) == false)
		{
			m_OccupyCollision.Delete(x, y, volume);
			return true;
		}
		return false;
	}
	
	inline bool	IsSpace(Int32 x, Int32 y, Int32 volume)
	{
		assert(core::Rect::PtInRect(x, y, volume) == true);
		if (core::Rect::PtInRect(x, y, volume) == false) return false;

		assert(volume > 0);
		if (volume <= 0) return false;

		return m_OccupyCollision.IsSpace(x, y, volume);
	}

	inline bool	Search(const core::Rect& rect, Int32 volume, core::Point& pos, const std::vector<core::Rect>* lstExceptRects/* = nullptr*/, core::FieldCollision* pColl)
	{
		if (rect.IsEmpty())
			return false;

		auto lstPoints = m_OccupyCollision.MaximalSquarePoints(rect, volume, lstExceptRects, pColl);
		if (lstPoints.empty())
			return false;

		core::Suffle(lstPoints);

		pos.SetPoint(lstPoints.at(0).x - volume + 1, lstPoints.at(0).y - volume + 1);

		return true;
	}
};
