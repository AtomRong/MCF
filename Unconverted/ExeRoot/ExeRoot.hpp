// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_EXE_ROOT_HPP__
#define __MCF_EXE_ROOT_HPP__

#include <MCF/StdMCF.hpp>
#include "AbstractExeRootClass.hpp"

extern "C" void __cdecl __MCFMain();

namespace MCF {
	extern AbstractExeRootClass *GetExeRootPtr();
	extern HINSTANCE GetHInstance();
}

#endif
