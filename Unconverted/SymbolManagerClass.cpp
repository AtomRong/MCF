// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "SymbolManagerClass.hpp"
using namespace MCF;

// ���캯��������������
SymbolManagerClass::SymbolManagerClass(HANDLE hProcess){
	xm_csLock.Enter();
		if(::SymInitialize(hProcess, nullptr, TRUE)){
			xm_bSymbolsLoaded = true;
			xm_SymbolInfo.SizeOfStruct = sizeof(xm_SymbolInfo);
			xm_SymbolInfo.MaxNameLen = MAX_SYM_NAME;
		} else {
			xm_bSymbolsLoaded = false;
		}
	xm_csLock.Leave();
}
SymbolManagerClass::~SymbolManagerClass(){
	xm_csLock.Enter();
		if(xm_bSymbolsLoaded){
			::SymCleanup(::GetCurrentProcess());
		}
	xm_csLock.Leave();
}

// �����Ǿ�̬��Ա������
LPCSTR SymbolManagerClass::SymbolFromAddr(DWORD dwAddr){
	LPCSTR pszRet;
	xm_csLock.Enter();
		if(!xm_bSymbolsLoaded){
			pszRet = nullptr;
		} else if(::SymFromAddr(::GetCurrentProcess(), dwAddr, nullptr, &xm_SymbolInfo)){
			pszRet = xm_SymbolInfo.Name;
		} else {
			pszRet = "";
		}
	xm_csLock.Leave();
	return pszRet;
}
bool SymbolManagerClass::AreSymbolsLoaded() const {
	return xm_bSymbolsLoaded;
}
