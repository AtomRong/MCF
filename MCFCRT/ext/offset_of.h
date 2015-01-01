// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013 - 2015, LH_Mouse. All wrongs reserved.

#ifndef MCF_CRT_OFFSET_OF_H_
#define MCF_CRT_OFFSET_OF_H_

#include "../env/_crtdef.h"
#include "_make_constant.h"

#ifdef __cplusplus
#	include <memory>
#	define __MCF_ADDRESS_OF(lval_)	\
	((unsigned char *)::std::addressof(lval_))
#else
#	define __MCF_ADDRESS_OF(lval_)	\
	((unsigned char *)&(lval_))
#endif

#define OFFSET_OF(s_, m_)	\
	(__MCF_CRT_MAKE_CONSTANT((MCF_STD size_t)(	\
		__MCF_ADDRESS_OF(((s_ *)(unsigned char *)1)->m_) - (unsigned char *)1)))

// ��Աָ��ת���ɾۺ�ָ�롣
#define DOWN_CAST(s_, m_, p_)	\
	((s_ *)((unsigned char *)(p_) - OFFSET_OF(s_, m_)))

#endif
