// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "NotifyIconManagerClass.hpp"
using namespace MCF;

// ��̬��Ա�������塣
std::atomic<std::size_t> NotifyIconManagerClass::xs_uIconCounter(0);

// ���캯��������������
// �����߱��븺�� hInitialIcon �Ĵ��������١�
NotifyIconManagerClass::NotifyIconManagerClass(HWND hOwner, UINT uMessageID, HICON hInitialIcon, LPCTSTR pszInitialTip, bool bInitiallyVisible) :
	xm_uIconID		(++xs_uIconCounter),
	xm_hOwner		(hOwner),
	xm_uMessageID	(uMessageID)
{
	ASSERT(hOwner != NULL);

	// ��֪ͨ�������ͼ�ꡣ
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_STATE;
	nidataNotifyIconData.uCallbackMessage	= xm_uMessageID;
	nidataNotifyIconData.hIcon				= hInitialIcon;
	_tcscpy_s(nidataNotifyIconData.szTip, pszInitialTip);
	nidataNotifyIconData.uVersion			= NOTIFYICON_VERSION;
	nidataNotifyIconData.dwState			= bInitiallyVisible ? 0 : (DWORD)NIS_HIDDEN;
	nidataNotifyIconData.dwStateMask		= NIS_HIDDEN;
	Shell_NotifyIcon(NIM_ADD, &nidataNotifyIconData);
	Shell_NotifyIcon(NIM_SETVERSION, &nidataNotifyIconData);
}
NotifyIconManagerClass::~NotifyIconManagerClass(){
	// ɾ��֪ͨ����ͼ�ꡣ
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	Shell_NotifyIcon(NIM_DELETE, &nidataNotifyIconData);
}

// �����Ǿ�̬��Ա������
void NotifyIconManagerClass::Hide(){
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_STATE;
	nidataNotifyIconData.dwState			= NIS_HIDDEN;
	nidataNotifyIconData.dwStateMask		= NIS_HIDDEN;
	Shell_NotifyIcon(NIM_MODIFY, &nidataNotifyIconData);
}
void NotifyIconManagerClass::Show(){
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_STATE;
	nidataNotifyIconData.dwState			= 0;
	nidataNotifyIconData.dwStateMask		= NIS_HIDDEN;
	Shell_NotifyIcon(NIM_MODIFY, &nidataNotifyIconData);
}
// �����߱��븺�� hNewIcon �Ĵ��������١�
void NotifyIconManagerClass::SetIcon(HICON hNewIcon){
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_ICON;
	nidataNotifyIconData.hIcon				= hNewIcon;
	Shell_NotifyIcon(NIM_MODIFY, &nidataNotifyIconData);
}
// ���������ͣʱ��ʾ�Ĺ�����ʾ��
// ע�ⲻ��������ʾ��
void NotifyIconManagerClass::SetTip(LPCTSTR pszNewTip){
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_TIP;
	if(pszNewTip == nullptr){
		nidataNotifyIconData.szTip[0]		= 0;
	} else {
		_tcscpy_s(nidataNotifyIconData.szTip, pszNewTip);
	}
	Shell_NotifyIcon(NIM_MODIFY, &nidataNotifyIconData);
}
// ��ʾ������ʾ��ע�ⲻ�ǹ�����ʾ��
// ������ʾ��һ�����⣬һ�����ݣ���ʾ�ӳ��Լ�ͼ�ꡣ
// dwFlags		NIIF_NONE		����ʾͼ�ꣻ
//				NIIF_ERROR		��ʾ��ɫ X ͼ�ꣻ
//				NIIF_INFO		��ʾ��ɫ i ͼ�ꣻ
//				NIIF_WARNING	��ʾ��ɫ ! ͼ�ꣻ
//				NIIF_USER		��ʾ hIcon ָ����ͼ�ꡣ
// �����߱��븺�� hIcon �Ĵ��������١�
void NotifyIconManagerClass::PopBalloon(LPCTSTR pszInfo, LPCTSTR pszTitle, UINT uTimeout, DWORD dwFlags, HICON hIcon){
	NOTIFYICONDATA nidataNotifyIconData;
#if(NTDDI_VERSION >= NTDDI_VISTA)
	nidataNotifyIconData.cbSize				= NOTIFYICONDATA_V3_SIZE;
#else
	nidataNotifyIconData.cbSize				= sizeof(nidataNotifyIconData);
#endif
	nidataNotifyIconData.hWnd				= xm_hOwner;
	nidataNotifyIconData.uID				= xm_uIconID;
	nidataNotifyIconData.uFlags				= NIF_INFO;
	if(pszInfo == nullptr){
		nidataNotifyIconData.szInfo[0] = 0;
	} else {
		_tcscpy_s(nidataNotifyIconData.szInfo, pszInfo);
	}
	if(pszTitle == nullptr){
		nidataNotifyIconData.szInfoTitle[0] = 0;
	} else {
		_tcscpy_s(nidataNotifyIconData.szInfoTitle, pszTitle);
	}
	nidataNotifyIconData.uTimeout			= uTimeout;
	nidataNotifyIconData.dwInfoFlags		= dwFlags;
	nidataNotifyIconData.hIcon				= hIcon;
	Shell_NotifyIcon(NIM_MODIFY, &nidataNotifyIconData);
}
