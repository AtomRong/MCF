// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_MODAL_DIALOG_CLASS_HPP__
#define __MCF_MODAL_DIALOG_CLASS_HPP__

#include "StdMCF.hpp"
#include "DialogBaseClass.hpp"

namespace MCF {
	class ModalDialogClass : public DialogBaseClass {
	private:
		using DialogBaseClass::xs_tlsCurrentInstance;
	private:
		using DialogBaseClass::xDlgProcStub;
		using DialogBaseClass::xDlgProc;
	public:
		ModalDialogClass(
			HINSTANCE			hInstance,
			int					nTemplateID,
			const DIPLITEM		*pDIPLItems = nullptr,
			std::size_t			uDIPLItemCount = 0,
			const std::size_t	*puStatusBarPartWidths = nullptr,
			std::size_t			uStatusBarPartCount = 0,
			std::size_t			uStatusBarMinHeight = 0
		);
		virtual ~ModalDialogClass();
	public:
		int DoModal(HWND hParent);
		void EndModal(int nExitCode = 0);
	};
}

#endif
