// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_LIB_HPP__
#define __MCF_LIB_HPP__

#include "StdMCF.hpp"

namespace MCF {
	extern void EnterGlobalCriticalSection();
	extern bool TryGlobalCriticalSection();
	extern void LeaveGlobalCriticalSection();
}

#endif
