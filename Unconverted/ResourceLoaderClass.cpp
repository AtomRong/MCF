// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "ResourceLoaderClass.hpp"
using namespace MCF;

// ���캯��������������
ResourceLoaderClass::ResourceLoaderClass(HMODULE hModule, int nResourceID, LPCTSTR pszResourceType){
	const HRSRC hResource = VERIFY(FindResource(hModule, MAKEINTRESOURCE(nResourceID), pszResourceType));

	xm_pData = VERIFY(LockResource(LoadResource(hModule, hResource)));
	xm_dwSize =  SizeofResource(hModule, hResource);
}
ResourceLoaderClass::~ResourceLoaderClass(){
}

// �����Ǿ�̬��Ա������
LPCVOID ResourceLoaderClass::GetDataPtr() const {
	return xm_pData;
}
DWORD ResourceLoaderClass::GetDataSize() const {
	return xm_dwSize;
}
