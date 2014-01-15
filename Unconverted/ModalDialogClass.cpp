// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "ModalDialogClass.hpp"
using namespace MCF;

// ���캯��������������
ModalDialogClass::ModalDialogClass(
	HINSTANCE			hInstance,
	int					nTemplateID,
	const DIPLITEM		*pDIPLItems,
	std::size_t			uDIPLItemCount,
	const std::size_t	*puStatusBarPartWidths,
	std::size_t			uStatusBarPartCount,
	std::size_t			uStatusBarMinHeight
) :
	DialogBaseClass(hInstance, nTemplateID, pDIPLItems, uDIPLItemCount, puStatusBarPartWidths, uStatusBarPartCount, uStatusBarMinHeight)
{
}
ModalDialogClass::~ModalDialogClass(){
}

// �����Ǿ�̬��Ա������
int ModalDialogClass::DoModal(HWND hParent){
	ASSERT(GetHWnd() == NULL);

	DialogBaseClass **const ppCurrentInstance = DialogBaseClass::xs_tlsCurrentInstance;
	ASSERT(*ppCurrentInstance == nullptr);
	*ppCurrentInstance = this;

	const int nExitCode = (int)::DialogBoxParam(GetHInstance(), MAKEINTRESOURCE(GetTemplateID()), hParent, &xDlgProcStub, 0);

	ASSERT(GetHWnd() == NULL);

	return nExitCode;
}

void ModalDialogClass::EndModal(int nExitCode){
	const HWND hDlg = GetHWnd();

	ASSERT(hDlg != NULL);

	::EndDialog(hDlg, (INT_PTR)nExitCode);
}
