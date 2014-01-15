// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "xThreadParasiteClass.hpp"
using namespace MCF;

// ���캯��������������
StandaloneWindowClass::xThreadParasiteClass::xThreadParasiteClass(StandaloneWindowClass *pMainWindow, bool bTranslateDialogMessages, std::intptr_t nContext, HACCEL hAcceleratorTable){
	ASSERT(pMainWindow != nullptr);

	xm_nContext = nContext;
	xm_hAcceleratorTable = hAcceleratorTable;

	xm_pMainWindow = pMainWindow;
	xm_bTranslateDialogMessages = bTranslateDialogMessages;

	xm_pPrevParasite = xm_pMainWindow->xm_pThreadParasite;
	xm_pMainWindow->xm_pThreadParasite = this;
}
StandaloneWindowClass::xThreadParasiteClass::~xThreadParasiteClass(){
	xm_pMainWindow->xm_pThreadParasite = xm_pPrevParasite;
}

// �����Ǿ�̬��Ա������
// �����������������ǶԴ������ĺ����ļ򵥰�װ��
bool StandaloneWindowClass::xThreadParasiteClass::xDoOnIdle(){
	if(xm_pMainWindow != nullptr){
		return xm_pMainWindow->xIdleLoop();
	}
	return false;
}
void StandaloneWindowClass::xThreadParasiteClass::xDoOnMessage(MSG *pMsg){
	bool bToDispatch = true;

	if(xm_pMainWindow != nullptr){
		if(xm_hAcceleratorTable != NULL){
			if(xm_pMainWindow->xPreTranslateAccelerator(pMsg) || (TranslateAccelerator(xm_pMainWindow->GetHWnd(), xm_hAcceleratorTable, pMsg) != FALSE)){
				bToDispatch = false;
			}
		}
		if(xm_bTranslateDialogMessages){
			if(xm_pMainWindow->xPreTranslateDialogMessage(pMsg) || (::IsDialogMessage(xm_pMainWindow->GetHWnd(), pMsg) != FALSE)){
				bToDispatch = false;
			}
		}
		if(xm_pMainWindow->xPreTranslateMessage(pMsg)){
			bToDispatch = false;
		}
	}

	if(bToDispatch){
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
	}

	if(xm_pMainWindow != nullptr){
		xm_pMainWindow->xPostDispatchMessage(pMsg);
	}
}

std::intptr_t StandaloneWindowClass::xThreadParasiteClass::GetContext() const {
	return xm_nContext;
}

int StandaloneWindowClass::xThreadParasiteClass::ParasitizeCurrentThread(){
	return AbstractMessageThreadClass::ParasitizeCurrentThread(false, false);
}
void StandaloneWindowClass::xThreadParasiteClass::QuitMessageLoop(int nExitCode){
	AbstractMessageThreadClass::QuitMessageLoop(nExitCode);
}
