// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#ifndef MCF_CRT_MAKE_CONSTANT_H_
#define MCF_CRT_MAKE_CONSTANT_H_

#include "../env/_crtdef.h"

#define __MCF_MAKE_CONSTANT(expr)	\
	(__builtin_constant_p(expr) ? (expr) : (expr))

#endif
