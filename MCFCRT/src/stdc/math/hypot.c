// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "_fpu.h"

#undef hypotf
#undef hypot
#undef hypotl

static inline long double fpu_hypot(long double x, long double y){
	long double xn, xm, yn, ym;
	xm = __MCFCRT_fxtract(&xn, x);
	ym = __MCFCRT_fxtract(&yn, y);
	if(xn > yn){
		return __MCFCRT_fscale(__MCFCRT_fsqrt(__MCFCRT_fsquare(xm) +
		                                      __MCFCRT_fsquare(__MCFCRT_fscale(y, -xn))),
		                       xn);
	} else {
		return __MCFCRT_fscale(__MCFCRT_fsqrt(__MCFCRT_fsquare(__MCFCRT_fscale(x, -yn)) +
		                                      __MCFCRT_fsquare(ym)),
		                       yn);
	}
}

float hypotf(float x, float y){
	return (float)fpu_hypot(x, y);
}
double hypot(double x, double y){
	return (double)fpu_hypot(x, y);
}
long double hypotl(long double x, long double y){
	return fpu_hypot(x, y);
}
