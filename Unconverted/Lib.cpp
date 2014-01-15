// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "Lib.hpp"
using namespace MCF;

namespace MCF {
	namespace {
		CriticalSection xGlobalCriticalSection;
	}

	extern void EnterGlobalCriticalSection(){
		xGlobalCriticalSection.Enter();
	}
	extern bool TryGlobalCriticalSection(){
		return xGlobalCriticalSection.Try();
	}
	extern void LeaveGlobalCriticalSection(){
		xGlobalCriticalSection.Leave();
	}
}
