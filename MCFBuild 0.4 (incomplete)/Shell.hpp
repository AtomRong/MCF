// MCF Build
// Copyleft 2014, LH_Mouse. All wrongs reserved.

#ifndef MCFBUILD_SHELL_HPP_
#define MCFBUILD_SHELL_HPP_

#include "../MCF/Core/String.hpp"

namespace MCFBuild {

extern unsigned int Shell(
	MCF::AnsiString &restrict strStdOut,
	MCF::AnsiString &restrict strStdErr,
	const MCF::WideStringObserver &wsoCommandLine
);

}

#endif