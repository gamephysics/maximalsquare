#pragma once

//================================================================
//	Singleton
//================================================================
namespace core
{
	template <class _Ty>
	class Singleton
	{
		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;

		static std::shared_ptr<_Ty>	instance_;
		static std::once_flag		onceflag_;

	public:
		Singleton() = default;
		virtual ~Singleton()
		{
			if (Singleton<_Ty>::instance_)
				Singleton<_Ty>::instance_.reset();
		}
		static std::shared_ptr<_Ty> Instance()
		{
			std::call_once(Singleton<_Ty>::onceflag_, []()
			{
				Singleton<_Ty>::instance_ = std::make_shared<_Ty>();
				if (!Singleton<_Ty>::instance_)
					throw std::bad_alloc();
			});
			return Singleton<_Ty>::instance_;
		}
		static std::shared_ptr<_Ty> Create()
		{
			return Singleton<_Ty>::Instance();
		}
	};

	template <class _Ty> std::shared_ptr<_Ty>	Singleton<_Ty>::instance_ = nullptr;
	template <class _Ty> std::once_flag			Singleton<_Ty>::onceflag_;


	//================================================================
	//	Singleton ThreadLocal
	//================================================================
	template <class _Ty>
	class SingletonThreadLocal
	{
		SingletonThreadLocal(SingletonThreadLocal const&) = delete;
		SingletonThreadLocal& operator=(SingletonThreadLocal const&) = delete;

		static thread_local std::shared_ptr<_Ty>	repository_;
		static thread_local std::once_flag			onceflag_;

	public:
		SingletonThreadLocal() = default;
		virtual ~SingletonThreadLocal()
		{
			if (SingletonThreadLocal<_Ty>::repository_)
				SingletonThreadLocal<_Ty>::repository_.reset();
		}
		static std::shared_ptr<_Ty> Instance()
		{
			std::call_once(SingletonThreadLocal<_Ty>::onceflag_, []()
			{
				SingletonThreadLocal<_Ty>::repository_ = std::make_shared<_Ty>();
				if (!SingletonThreadLocal<_Ty>::repository_)
					throw std::bad_alloc();
			});
			return SingletonThreadLocal<_Ty>::repository_;
		}
		static std::shared_ptr<_Ty> Create()
		{
			return SingletonThreadLocal<_Ty>::Instance();
		}
	};

	template <class _Ty> thread_local std::shared_ptr<_Ty>	SingletonThreadLocal<_Ty>::repository_ = nullptr;
	template <class _Ty> thread_local std::once_flag		SingletonThreadLocal<_Ty>::onceflag_;
};




