// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#ifndef MCF_RANDOM_ISAAC_GENERATOR_HPP_
#define MCF_RANDOM_ISAAC_GENERATOR_HPP_

#include "RandomSeed.hpp"

namespace MCF {

class IsaacGenerator {
public:
	static std::uint32_t GlobalGet() noexcept;

private:
	std::uint32_t xm_u32Internal[256];
	std::uint32_t xm_u32A;
	std::uint32_t xm_u32B;
	std::uint32_t xm_u32C;

	std::uint32_t xm_u32Results[256];
	std::uint32_t xm_u32Read;

public:
	constexpr IsaacGenerator() noexcept
		: xm_u32Internal(), xm_u32A(0), xm_u32B(0), xm_u32C(0)
		, xm_u32Results(), xm_u32Read(0)
	{
	}
	explicit IsaacGenerator(std::uint32_t u32Seed) noexcept {
		Init(u32Seed);
	}
	explicit IsaacGenerator(const std::uint32_t (&au32Seed)[8]) noexcept {
		Init(au32Seed);
	}

private:
	void xRefreshInternal() noexcept;

public:
	void Init(std::uint32_t u32Seed = GenerateRandSeed()) noexcept;
	void Init(const std::uint32_t (&au32Seed)[8]) noexcept;

	std::uint32_t Get() noexcept;
};

}

#endif