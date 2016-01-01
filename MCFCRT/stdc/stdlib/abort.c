// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "../../env/bail.h"
#include "../../env/last_error.h"
#include <setjmp.h>

jmp_buf *__MCF_CRT_abort_hook_jmpbuf = nullptr;

__attribute__((__used__))
_Noreturn void __wrap_abort(){
	if(__MCF_CRT_abort_hook_jmpbuf){
		longjmp(*__MCF_CRT_abort_hook_jmpbuf, ERROR_PROCESS_ABORTED);
	}

	MCF_CRT_Bail(L"应用程序调用了 abort()。");
}

_Noreturn
__attribute__((__alias__("__wrap_abort")))
void abort(void);
