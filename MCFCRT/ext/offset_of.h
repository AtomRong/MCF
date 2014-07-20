// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013 - 2014 LH_Mouse. All wrongs reserved.

#ifndef MCF_CRT_OFFSET_OF_H_
#define MCF_CRT_OFFSET_OF_H_

#include "../env/_crtdef.h"

#ifdef __cplusplus
#	define __MCF_ADDRESS_OF(lval)	((unsigned char *)&reinterpret_cast<const volatile unsigned char &>(lval))
#else
#	define __MCF_ADDRESS_OF(lval)	((unsigned char *)&(lval))
#endif

#define OFFSET_OF(s, m)	\
	((MCF_STD size_t)(__MCF_ADDRESS_OF(((s *)(unsigned char *)1)->m) - (unsigned char *)1))

// ��Աָ��ת���ɾۺ�ָ�롣
#define DOWN_CAST(s, m, p)	\
	((s *)((unsigned char *)(p) - OFFSET_OF(s, m)))

#endif
