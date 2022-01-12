#pragma once

namespace core
{
	class FieldCollision
	{
		Int32	width_ = 0;
		Int32	height_ = 0;

		static const Int64 npos;		//	default npos == -1
										//
		core::Mask	xBoundaryPoints;
		core::Mask	yBoundaryPoints;

		std::recursive_mutex	lock_;

	public:
		typedef std::shared_ptr<FieldCollision>		SharedPtr;
		typedef std::weak_ptr<FieldCollision>		WeakPtr;

		auto	GetWidth() const { return width_; }
		auto	GetHeight() const { return height_; }
		auto	SetWidth(const decltype(width_)& o) { width_ = o; }
		void	SetHeight(const decltype(height_)& o) { height_ = o; }
		//
		void	InitInstance(const Int32& width, const Int32& height);
		void	CopyToFieldCollision(FieldCollision::SharedPtr	newCollision);
		void	CopyToFieldCollision(FieldCollision*			newCollision);
		//	[충돌맵]
		void	SetAt(Int32 x, Int32 y);
		void	ClrAt(Int32 x, Int32 y);
		void	SetRange(Int32 xMin, Int32 yMin, Int32 xMax, Int32 yMax, bool flags);
		void	SetHorizon(Int32 x1, Int32 x2, Int32 y, bool flags);

		//	[객체충돌]
		bool	Insert(Int32 x, Int32 y, Int32 volume);
		bool	Delete(Int32 x, Int32 y, Int32 volume);
		bool	Delete(Int32 x, Int32 y);
		bool	IsSpace(Int32 x, Int32 y, Int32 volume, const std::vector<core::Rect>* lstExceptRects = nullptr) const;

		bool	Insert(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume);
		bool	Delete(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume);
		bool	IsSpace(Int32 x, Int32 y, Int32 xVolume, Int32 yVolume, const core::Rect* prcExcept = nullptr) const;

		//	[일반함수]
		bool	IsCollision(Int32 x, Int32 y);		//	<-- 길찾기용으로 사용하세요..
		bool	IsEmpty(Int32 x, Int32 y, Int32 width, Int32 height) const;
		void	Dump();
		//
		Int32	GetPosX(Int32 x, Int32 y) const;
		Int32	GetPosY(Int32 x, Int32 y) const;
		size_t	GetValueX(Int32 x, Int32 y);
		size_t	GetValueY(Int32 x, Int32 y);

		// [검색]
		static 	thread_local std::vector<Int16>	dpp_;
		std::vector<core::Point> MaximalSquarePoints(const core::Rect& rect, Int32 n, const std::vector<core::Rect>* lstExceptRects = nullptr, FieldCollision* pColl = nullptr);
	};
};
