// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "VersionLoaderClass.hpp"
using namespace MCF;

// ��̬��Ա������
BOOL CALLBACK VersionLoaderClass::xEnumVersionResNameProc(HMODULE /* hModule */, LPCTSTR pszType, LPTSTR pszName, LPARAM lParam){
	UNREF_PARAM(pszType);

	ASSERT(IS_INTRESOURCE(pszType) && (pszType == RT_VERSION));

	VersionLoaderClass *const pVersionLoaderClassInstance = (VersionLoaderClass *)lParam;
	ASSERT(pVersionLoaderClassInstance != nullptr);

	pVersionLoaderClassInstance->xm_pszResourceName = pszName;

	// ֻ��õ�һ���汾��Դ��
	return FALSE;
}

// ���캯��������������
VersionLoaderClass::VersionLoaderClass(HMODULE hModule){
	xm_pszResourceName = nullptr;
	EnumResourceNames(hModule, RT_VERSION, &xEnumVersionResNameProc, (LONG_PTR)this);
	VERIFY(xm_pszResourceName != nullptr);

	const HRSRC hVersionResource = VERIFY(FindResource(hModule, xm_pszResourceName, RT_VERSION));
	const LPCVOID pFileVersionBuffer = VERIFY(LockResource(LoadResource(hModule, hVersionResource)));

	VS_FIXEDFILEINFO *pFixedFileInfo;
	UINT uFixedFileInfoSize;
	VerQueryValue(pFileVersionBuffer, _T("\\"), (LPVOID *)&pFixedFileInfo, &uFixedFileInfoSize);

	ASSERT(uFixedFileInfoSize != 0);

	xm_arwVersionBuffer[0] = HIWORD(pFixedFileInfo->dwFileVersionMS);
	xm_arwVersionBuffer[1] = LOWORD(pFixedFileInfo->dwFileVersionMS);
	xm_arwVersionBuffer[2] = HIWORD(pFixedFileInfo->dwFileVersionLS);
	xm_arwVersionBuffer[3] = LOWORD(pFixedFileInfo->dwFileVersionLS);
}
VersionLoaderClass::~VersionLoaderClass(){
}

// �����Ǿ�̬��Ա������
const WORD &VersionLoaderClass::operator[](std::size_t uIndex) const {
	return xm_arwVersionBuffer[uIndex];
}
