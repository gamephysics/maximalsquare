#pragma once

namespace core
{
	struct Point;
	struct Rect;
	struct Occupation;

	struct Point
	{
		Int32  x = -1;
		Int32  y = -1;
	public:
		Point()
		{
			SetPoint(-1, -1);
		}
		Point(Int32 initX, Int32 initY)
		{
			SetPoint(initX, initY);
		}
		Point(const Point& initPoint)
		{
			*this = initPoint;
		}
		//	Operations
		void SetPoint(Int32 x, Int32 y)
		{
			this->x = x;
			this->y = y;
		}
		bool IsEmpty() const
		{
			return (x == -1 || y == -1);
		}
		bool operator==(const Point& point) const
		{
			return (x == point.x && y == point.y);
		}
		bool operator!=(const Point& point) const
		{
			return (x != point.x || y != point.y);
		}
		Point& operator=(const Point& point)
		{
			SetPoint(point.x, point.y);
			return (*this);
		}

		static Point npos;	//	default (-1, -1)

		bool operator!() const	{ return IsEmpty() == true; }
		operator bool()			{ return IsEmpty() == false; }
	};

	//==================================================================
	// y
	// ^
	// |	+--- right,top
	// |	|         |
	// |	|         | 
	// | left,bottom--+
	// +--------------------------> x
	//==================================================================
	struct Rect
	{
		Int32 left = -1, bottom = -1, right = -1, top = -1;
	public:
		Rect()
		{
			this->SetRectEmpty();
		}
		Rect(Int32 x1, Int32 y1, Int32 x2, Int32 y2)
		{
			this->SetRect(x1, y1, x2, y2);
		}
		Rect(Int32 x1, Int32 y1, Int32 volume)
		{
			this->SetRectVolume(x1, y1, volume);
		}

		//	Operations
		void SetRect(Int32 x1, Int32 y1, Int32 x2, Int32 y2)
		{
			left = x1; bottom = y1; right = x2; top = y2;
			this->NormalizeRect();
		}
		void SetRect(const Rect& srcRect)
		{
			left = srcRect.left; bottom = srcRect.bottom; right = srcRect.right; top = srcRect.top;
			this->NormalizeRect();
		}
		void SetRectVolume(Int32 x1, Int32 y1, Int32 volume)
		{
			left = x1; bottom = y1; right = x1 + volume - 1; top = y1 + volume - 1;
			this->NormalizeRect();
		}
		void SetRectVolume(Int32 x1, Int32 y1, Int32 xVolume, Int32 yVolume)
		{
			left = x1; bottom = y1; right = x1 + xVolume - 1; top = y1 + yVolume - 1;
			this->NormalizeRect();
		}
		//	초기화
		void SetRectEmpty()
		{
			this->SetRect(-1, -1, -1, -1);
		}
		void NormalizeRect()
		{
			if (left > right)	std::swap(left, right);
			if (bottom > top)	std::swap(bottom, top);
		}
		bool IsEmpty() const
		{
			return (left == -1 && bottom == -1 && right == -1 && top == -1);
		}
		Int32 GetLos() const
		{
			assert(GetWidth() == GetHeight());
			return (right - left + 1);
		}
		Int32 GetWidth() const
		{
			return (right - left + 1);
		}
		Int32 GetHeight() const
		{
			return (top - bottom + 1);
		}
		Point GetLeftBottom() const
		{
			return Point(this->left, this->bottom);
		}
		Point GetLeftTop() const
		{
			return Point(this->left, this->top);
		}
		Point GetRightBottom() const
		{
			return Point(this->right, this->bottom);
		}
		Point GetRightTop() const
		{
			return Point(this->right, this->top);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////
		//	겹치는지 검사 (원본수정 없음)
		bool IsIntersectRect(const Rect& srcRect) const
		{
			if (this->left	> srcRect.right||
				this->bottom> srcRect.top  ||
				this->right < srcRect.left || 
				this->top	< srcRect.bottom)
				return  false;
			return true;

		}
		bool IntersectRect(const Rect& srcRect, Rect& resRect) const
		{
			if (srcRect.IsEmpty() || this->IsEmpty())
				return false;

			if (IsIntersectRect(srcRect))
			{
				resRect.left	= std::max(this->left,	srcRect.left);
				resRect.bottom	= std::max(this->bottom,srcRect.bottom);
				resRect.top		= std::min(this->top,	srcRect.top);
				resRect.right	= std::min(this->right, srcRect.right);
				assert(resRect.bottom <= resRect.top && resRect.left <= resRect.right);
				return true;
			}
			return false;
		}
		
		Int32 Area() const
		{
			return GetWidth() * GetHeight();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		//	포함하는지 검사
		bool PtInRect(const Point& point) const
		{
			if (this->left	> point.x ||
				this->bottom> point.y ||
				this->right < point.x ||
				this->top	< point.y)
				return  false;
			return true;
		}
		bool PtInRect(Int32 x, Int32 y) const
		{
			return this->PtInRect(Point(x, y));
		}
		bool PtInRect(Int32 x, Int32 y, Int32 volume) const
		{
			return this->PtInRect(Point(x, y)) && this->PtInRect(Point(x + volume - 1, y + volume - 1));
		}		
		bool PtInRect(const Rect& rect) const
		{
			return this->PtInRect(Point(rect.left, rect.bottom)) && this->PtInRect(Point(rect.right, rect.top));
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		//	확장 / 축소
		bool UnionRect(const Rect& srcRect)
		{
			if (srcRect.IsEmpty())
				return true;

			if (this->IsEmpty())
			{
				*this = srcRect;
				return true;
			}
			
			left	= std::min(this->left,	srcRect.left);
			bottom	= std::min(this->bottom,srcRect.bottom);
			top		= std::max(this->top,	srcRect.top);
			right	= std::max(this->right, srcRect.right);
			return true;
		}
		bool UnionRect(const Point& point)
		{
			return this->UnionRect(Rect(point.x, point.y, point.x, point.y));
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		Rect& operator=(const Rect& srcRect)
		{
			this->SetRect(srcRect);
			return (*this);
		}
		Rect& operator=(const Point& srcPoint)
		{
			this->SetRect(srcPoint.x, srcPoint.y, srcPoint.x, srcPoint.y);
			return (*this);
		}
		bool operator==(const Rect& rect) const
		{
			return (left == rect.left && bottom == rect.bottom && right == rect.right && top == rect.top);
		}
		bool operator!=(const Rect& rect) const
		{
			return (left != rect.left || bottom != rect.bottom || right != rect.right || top != rect.top);
		}
		bool Compare(const Rect& rect) const
		{
			return operator==(rect);
		}
	};

};

__declspec(selectany) core::Point	core::Point::npos;

namespace std
{
	template <>
	struct hash<core::Point>
	{
		size_t operator()(const core::Point& value) const
		{
			size_t	res = 17;

			res = (res * 31) + hash<UInt64>()(value.x);
			res = (res * 31) + hash<UInt64>()(value.y);
			return res;
		}
	};
};
