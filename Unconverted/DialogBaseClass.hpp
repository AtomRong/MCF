// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_DIALOG_BASE_CLASS_HPP__
#define __MCF_DIALOG_BASE_CLASS_HPP__

#include "StdMCF.hpp"
#include <vector>
#include "Memory.hpp"
#include "StandaloneWindowClass.hpp"

namespace MCF {
	class DialogBaseClass : public StandaloneWindowClass {
	private:
		class xDlgItemPosLockClass;
	public:
		// �ؼ�λ�����Ĳ�����Ӧ�����ݶԻ�������Ԥ���塣ÿ���ؼ�һ����
		typedef struct xtagDIPLItem {
			int nCtrlID;
			bool bLeftEdgeLockedUponRight;		// false ������������߽磬true �����������ұ߽硣
			bool bTopEdgeLockedUponBottom;		// false �����������ϱ߽磬true �����������±߽硣
			bool bRightEdgeLockedUponRight;		// false ������������߽磬true �����������ұ߽硣
			bool bBottomEdgeLockedUponBottom;	// false �����������ϱ߽磬true �����������±߽硣
		} DIPLITEM;
	protected:
		static TLSManagerTemplate<DialogBaseClass *> xs_tlsCurrentInstance;
	protected:
		static INT_PTR CALLBACK xDlgProcStub(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static INT_PTR CALLBACK xDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		const HINSTANCE xm_hInstance;
		const int xm_nTemplateID;
		const std::vector<DIPLITEM> xm_vecDIPLItems;

		xDlgItemPosLockClass *xm_pDlgItemPosLock;
	public:
		DialogBaseClass(
			HINSTANCE			hInstance,
			int					nTemplateID,
			const DIPLITEM		*pDIPLItems,
			std::size_t			uDIPLItemCount,
			const std::size_t	*puStatusBarPartWidths,
			std::size_t			uStatusBarPartCount,
			std::size_t			uStatusBarMinHeight
		);
		virtual ~DialogBaseClass();
	protected:
		virtual LRESULT xOnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		virtual bool xOnInitDialog(HWND hDefControl, LPARAM lInitParam);
		virtual void xOnDestroy() override;
		virtual void xOnSize(int nType, unsigned int uNewWidth, unsigned int uNewHeight) override;
	protected:
		void xMakeSubclassDlgItem(WindowBaseClass &which, int nItemID);
	public:
		HINSTANCE GetHInstance() const;
		int GetTemplateID() const;
		HWND GetDlgItemHWnd(int nControlID) const;

		bool IsDlgItemVisible(int nDlgItemID) const;
		void ShowDlgItem(int nDlgItemID, bool bToShow);
		bool IsDlgItemEnabled(int nDlgItemID) const;
		void EnableDlgItem(int nDlgItemID, bool bToEnable);
		std::size_t GetDlgItemTextLength(int nDlgItemID) const;
		TString GetDlgItemText(int nDlgItemID) const;
		void SetDlgItemText(int nDlgItemID, LPCTSTR pszSetToWhich);
		int IsDlgButtonChecked(int nDlgItemID) const;
		void CheckDlgButton(int nDlgItemID, int nCheck);

		RECT GetDlgItemAbsRect(int nDlgItemID) const;
		RECT GetDlgItemRect(int nDlgItemID) const;

		LRESULT SendDlgItemMessage(int nDlgItemID, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
		void PostDlgItemMessage(int nDlgItemID, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	};
}

#endif
