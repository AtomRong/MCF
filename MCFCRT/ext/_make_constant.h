// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013 - 2015, LH_Mouse. All wrongs reserved.

#ifndef MCF_CRT_MAKE_CONSTANT_H_
#define MCF_CRT_MAKE_CONSTANT_H_

#include "../env/_crtdef.h"

#define __MCF_CRT_MAKE_CONSTANT(expr_)	\
	(__builtin_constant_p(expr_) ? (expr_) : (expr_))

#endif
