// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2015, LH_Mouse. All wrongs reserved.

#ifndef MCF_THREAD_EVENT_HPP_
#define MCF_THREAD_EVENT_HPP_

#include "../Utilities/Noncopyable.hpp"
#include "Mutex.hpp"
#include "ConditionVariable.hpp"
#include <cstddef>
#include <cstdint>

namespace MCF {

class Event : NONCOPYABLE {
private:
	mutable Mutex x_mtxGuard;
	mutable ConditionVariable x_cvWaiter;
	bool x_bSet;

public:
	explicit Event(bool bInitSet) noexcept
		: x_mtxGuard(), x_cvWaiter(), x_bSet(bInitSet)
	{
	}

public:
	bool Wait(std::uint64_t u64UntilFastMonoClock) const noexcept;
	void Wait() const noexcept;
	bool IsSet() const noexcept;
	bool Set() noexcept;
	bool Reset() noexcept;
};

}

#endif
