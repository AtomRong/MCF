// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_UTILITIES_HPP__
#define __MCF_UTILITIES_HPP__

#include "String.hpp"
#include <cstddef>

namespace MCF {

extern UTF16String GetWin32ErrorDesc(unsigned long ulErrorCode);

extern unsigned int GetUNIXTime() noexcept;
extern std::uint32_t GenRandSeed() noexcept;
extern std::uint64_t GetHiResTimer() noexcept;

template<typename T>
inline void __attribute__((always_inline)) ZeroObject(T &dst) noexcept {
	__builtin_memset(&dst, 0, sizeof(dst));
}

}

#endif
