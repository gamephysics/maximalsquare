#pragma once

namespace core
{
	class CRandom : public core::Singleton<CRandom>
	{
		std::random_device			device_;
		std::default_random_engine	generator_;	//	default mt19937

	public:
		typedef std::shared_ptr<CRandom>	SharedPtr;
		typedef std::weak_ptr<CRandom>		WeakPtr;

		CRandom()
		{
			generator_.seed(device_());
		}

		template <typename _Ty, typename = std::enable_if_t<core::is_vector<_Ty>::value>>
		void suffle(_Ty& repo)
		{
			if (repo.size() >= 2)
				std::shuffle(repo.begin(), repo.end(), generator_);
		}
	};

	template <typename _Ty, typename = std::enable_if_t<core::is_vector<_Ty>::value>>
	void Suffle(_Ty& repo)
	{
		return CRandom::Instance()->suffle(repo);
	}
};