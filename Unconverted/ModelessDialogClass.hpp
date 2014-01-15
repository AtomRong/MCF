// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_MODELESS_DIALOG_CLASS_HPP__
#define __MCF_MODELESS_DIALOG_CLASS_HPP__

#include "StdMCF.hpp"
#include "DialogBaseClass.hpp"

namespace MCF {
	class ModelessDialogClass : public DialogBaseClass {
	private:
		using DialogBaseClass::xs_tlsCurrentInstance;
	private:
		using DialogBaseClass::xDlgProcStub;
		using DialogBaseClass::xDlgProc;
	public:
		ModelessDialogClass(
			HINSTANCE			hInstance,
			int					nTemplateID,
			const DIPLITEM		*pDIPLItems = nullptr,
			std::size_t			uDIPLItemCount = 0,
			const std::size_t	*puStatusBarPartWidths = nullptr,
			std::size_t			uStatusBarPartCount = 0,
			std::size_t			uStatusBarMinHeight = 0
		);
		virtual ~ModelessDialogClass();
	public:
		void Create(HWND hParent);
		void Destroy();
	};
}

#endif
