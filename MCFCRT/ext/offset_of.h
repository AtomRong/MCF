// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2014. LH_Mouse. All wrongs reserved.

#ifndef MCF_CRT_OFFSET_OF_H_
#define MCF_CRT_OFFSET_OF_H_

#include "../env/_crtdef.h"

#ifdef __cplusplus
#	define __MCF_ADDRESS_OF		&(char &)
#else
#	define __MCF_ADDRESS_OF
#endif

#define OFFSET_OF(s, m)	\
	((MCF_STD size_t)((MCF_STD uintptr_t)__MCF_ADDRESS_OF(((s *)(MCF_STD uintptr_t)1)->m) - (MCF_STD uintptr_t)1))

// ��Աָ��ת���ɾۺ�ָ�롣
#define DOWN_CAST(s, m, p)	\
	((s *)((MCF_STD uintptr_t)(p) - OFFSET_OF(s, m)))

#endif
