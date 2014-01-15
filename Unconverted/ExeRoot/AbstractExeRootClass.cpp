// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "AbstractExeRootClass.hpp"
#include <vector>
using namespace MCF;

// ��̬��Ա�������塣
std::atomic<AbstractExeRootClass *> AbstractExeRootClass::xs_pLastLatentExeRoot(nullptr);

// ���캯��������������
AbstractExeRootClass::AbstractExeRootClass(){
	xm_pNext = xs_pLastLatentExeRoot.exchange(this);

	xm_pvecArgVector = nullptr;
}
AbstractExeRootClass::~AbstractExeRootClass(){
}

// �����Ǿ�̬��Ա������
int AbstractExeRootClass::xThreadProc(){
	ASSERT(xm_pvecArgVector != nullptr);

	return xMCFMain(xm_pvecArgVector->size() - 1, xm_pvecArgVector->data(), xm_nCmdShow);
}

HINSTANCE AbstractExeRootClass::GetHInstance() const {
	return xm_hInstance;
}
