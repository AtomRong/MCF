// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_CRITICAL_SECTION_HPP__
#define __MCF_CRITICAL_SECTION_HPP__

#include "NoCopy.hpp"
#include <memory>
#include <utility>

namespace MCF {

class CriticalSection : NO_COPY {
private:
	class xDelegate;

	struct xUnlocker {
		void operator()(xDelegate *pDelegate) const noexcept {
			xUnlock(pDelegate);
		}
	};
private:
	static void xUnlock(xDelegate *pDelegate) noexcept;
public:
	typedef std::unique_ptr<xDelegate, xUnlocker> LockHolder;
private:
	const std::unique_ptr<xDelegate> xm_pDelegate;
public:
	CriticalSection(unsigned long ulSpinCount = 0x400);
	~CriticalSection();
public:
	LockHolder Try() noexcept;
	LockHolder Lock() noexcept;
};

}

#define CRITICAL_SECTION_SCOPE(cs)	\
	for(auto __MCF_LOCK__ = ::std::make_pair((cs).Lock(), true);\
		__MCF_LOCK__.second;	\
		__MCF_LOCK__.second = false	\
	)

#endif
