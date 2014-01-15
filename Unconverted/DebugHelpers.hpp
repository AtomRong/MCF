// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_DEBUG_HELPERS_HPP__
#define __MCF_DEBUG_HELPERS_HPP__

#include "Utilities.hpp"
#include "GenericString.hpp"
#include <tchar.h>
#include <windows.h>

namespace MCF {
	inline TString GetErrorDescription(DWORD dwErrorCode){
		LPVOID pDescriptionBuffer;
		TString strRet;

		if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, dwErrorCode, 0, (LPTSTR)&pDescriptionBuffer, 0, nullptr) != 0){
			strRet = (LPCTSTR)pDescriptionBuffer;
		}
		::LocalFree((HLOCAL)pDescriptionBuffer);

		return strRet;
	}

	inline void __attribute__((noreturn)) __Bail(int nExitCode){
		::TerminateProcess(::GetCurrentProcess(), (std::size_t)nExitCode);
		throw;
	}

	inline int __DebugMessageBoxA(LPCSTR pszText, LPCSTR pszCaption, UINT uType){
		class MessageBoxThreadCreatorClass {
		private:
			static DWORD WINAPI xMessageBoxThread(LPVOID pParam){
				MessageBoxThreadCreatorClass *const pCreator = (MessageBoxThreadCreatorClass *)pParam;

				MSG MsgDummy;
				::PeekMessage(&MsgDummy, NULL, WM_USER, WM_USER, PM_NOREMOVE);

				pCreator->xm_nExitCode = ::MessageBoxA(NULL, pCreator->xm_pszText, pCreator->xm_pszCaption, pCreator->xm_uType);

				return 0;
			}
		private:
			const LPCSTR xm_pszText;
			const LPCSTR xm_pszCaption;
			const UINT xm_uType;
			int xm_nExitCode;
		public:
			MessageBoxThreadCreatorClass(LPCSTR pszText, LPCSTR pszCaption, UINT uType) :
				xm_pszText		(pszText),
				xm_pszCaption	(pszCaption),
				xm_uType		(uType),
				xm_nExitCode	(MB_OK)
			{
				const HANDLE hMessageBoxThread = ::CreateThread(nullptr, 0, &xMessageBoxThread, (LPVOID)this, 0, nullptr);
				::WaitForSingleObject(hMessageBoxThread, INFINITE);
				::CloseHandle(hMessageBoxThread);
			}
			~MessageBoxThreadCreatorClass(){
			}
		private:
			MessageBoxThreadCreatorClass(const MessageBoxThreadCreatorClass &);
			void operator=(const MessageBoxThreadCreatorClass &);
		public:
			int GetExitCode() const {
				return xm_nExitCode;
			}
		};

		return MessageBoxThreadCreatorClass(pszText, pszCaption, uType).GetExitCode();
	}
	inline int __DebugMessageBoxW(LPCWSTR pwszText, LPCWSTR pwszCaption, UINT uType){
		class MessageBoxThreadCreatorClass {
		private:
			static DWORD WINAPI xMessageBoxThread(LPVOID pParam){
				MessageBoxThreadCreatorClass *const pCreator = (MessageBoxThreadCreatorClass *)pParam;

				MSG MsgDummy;
				::PeekMessage(&MsgDummy, NULL, WM_USER, WM_USER, PM_NOREMOVE);

				pCreator->xm_nExitCode = ::MessageBoxW(NULL, pCreator->xm_pwszText, pCreator->xm_pwszCaption, pCreator->xm_uType);

				return 0;
			}
		private:
			const LPCWSTR xm_pwszText;
			const LPCWSTR xm_pwszCaption;
			const UINT xm_uType;
			int xm_nExitCode;
		public:
			MessageBoxThreadCreatorClass(LPCWSTR pwszText, LPCWSTR pwszCaption, UINT uType) :
				xm_pwszText	(pwszText),
				xm_pwszCaption	(pwszCaption),
				xm_uType		(uType),
				xm_nExitCode	(MB_OK)
			{
				const HANDLE hMessageBoxThread = ::CreateThread(nullptr, 0, &xMessageBoxThread, (LPVOID)this, 0, nullptr);
				::WaitForSingleObject(hMessageBoxThread, INFINITE);
				::CloseHandle(hMessageBoxThread);
			}
			~MessageBoxThreadCreatorClass(){
			}
		private:
			MessageBoxThreadCreatorClass(const MessageBoxThreadCreatorClass &);
			void operator=(const MessageBoxThreadCreatorClass &);
		public:
			int GetExitCode() const {
				return xm_nExitCode;
			}
		};

		return MessageBoxThreadCreatorClass(pwszText, pwszCaption, uType).GetExitCode();
	}

// ======================================================================================================================================================
// ======================================================================================================================================================

	// ���ԡ�
	// �÷��ͱ�׼��� assert() ��������
#ifndef NDEBUG
	extern inline void __ShowAssertion(const char *pszExpression, const char *pszFile, long lLine, const char *pszDescription){
		const DWORD dwLastError = ::GetLastError();
		if(__DebugMessageBoxA(
			MBString().Format(
				"���Զ���ʧ�ܣ�\n\n���ʽ��%s\n�ļ�����%s\n�кš���%ld\n��������%s\n\n������ȷ������ֹӦ�ó��򣬵�����ȡ��������Ӧ�ó���",
				pszExpression,
				pszFile,
				lLine,
				pszDescription
			),
			"����ʧ��",
			MB_OKCANCEL | MB_ICONSTOP | MB_TASKMODAL | MB_TOPMOST
		) == IDOK){
			__Bail(-3);
		} else {
			::SetLastError(dwLastError);
			::DebugBreak();
		}
	}
#endif	// NDEBUG

#ifndef NDEBUG
#	define ASSERT(exp)					((void)(!!(exp) || (::MCF::__ShowAssertion(#exp, __FILE__, __LINE__, ""), false)))
#	define ASSERTD(exp, description)	((void)(!!(exp) || (::MCF::__ShowAssertion(#exp, __FILE__, __LINE__, description), false)))
#else	// NDEBUG
#	define ASSERT(exp)					((void)0)
#	define ASSERTD(exp, description)	((void)0)
#endif	// NDEBUG

// ======================================================================================================================================================
// ======================================================================================================================================================

	// ��֤��
	// ��֤�Ͷ��Ե�������������
	// 1. ��֤��һ�������������������֤��ֵת��Ϊ bool ���Ϊ true���򷵻ظ�ֵ����
	//    ��˿����������﷨��int *pnSomeInt = VERIFY(new int());
	// 2. ������ Release ��������Ч����֤����Ч����֤ʧ��ʱ���ṩ����ѡ�
#ifndef NDEBUG
	template<typename T>
	extern inline T &&__ShowVerification(T &&arg, const char *pszExpression, const char *pszFile, long lLine, const char *pszDescription){
		if(!arg){
			const DWORD dwLastError = ::GetLastError();
			if(__DebugMessageBoxA(
				MBString().Format(
					"��֤ʧ�ܣ�\n\n���ʽ��%s\n�ļ�����%s\n�кš���%ld\n��������%s\n\n������룺%lu\n����������%s\n������ȷ������ֹӦ�ó��򣬵�����ȡ��������Ӧ�ó���",
					pszExpression,
					pszFile,
					lLine,
					pszDescription,
					(unsigned long)dwLastError,
					(LPCSTR)MBString().ConvFrom(GetErrorDescription(dwLastError))
				),
				"��֤ʧ��",
				MB_OKCANCEL | MB_ICONSTOP | MB_TASKMODAL | MB_TOPMOST
			) == IDOK){
				__Bail(-3);
			} else {
				::SetLastError(dwLastError);
				::DebugBreak();
			}
		}
		return std::forward<T>(arg);
	}
#else	// NDEBUG
	template<typename T>
	extern inline T &&__ShowVerification(T &&arg, const char *pszExpression, const char *pszFile, std::size_t uLine, const char *pszDescription){
		if(!arg){
			const DWORD dwLastError = ::GetLastError();
			__DebugMessageBoxA(
				MBString().Format(
					"Ӧ�ó�������ʱ���ִ�������ϵ�������˽��йش�����Ľ��������\n���ʽ��%s\n\n�ļ�����%s\n�кš���%d\n��������%s\n\n������룺%lu\n����������%s\n������ȷ������ֹӦ�ó���",
					pszExpression,
					pszFile,
					uLine,
					pszDescription,
					(unsigned long)dwLastError,
					(LPCSTR)MBString().ConvFrom(GetErrorDescription(dwLastError))
				),
				"��֤ʧ��",
				MB_OK | MB_ICONSTOP | MB_TASKMODAL | MB_TOPMOST
			);
			__Bail(-3);
		}
		return std::forward<T>(arg);
	}
#endif	// NDEBUG
}

#define VERIFY(exp)						(::MCF::__ShowVerification((exp), #exp, __FILE__, __LINE__, ""))
#define VERIFYD(exp, description)		(::MCF::__ShowVerification((exp), #exp, __FILE__, __LINE__, description))

// ======================================================================================================================================================
// ======================================================================================================================================================

// ���Ծ����
// �������ڳ�������ʱ��ʱ����������ֵ��
// �÷��� JavaScript �е� alert() ���ơ�
// �����ṩ ANSI �� Unicode �ַ��������ذ汾��ʹ��ʱ���ؼ��� xT()��
inline void ALERT(LPCSTR pszMessage){
	::MCF::__DebugMessageBoxA(pszMessage, "Debug Alert", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
}
inline void ALERTF(LPCSTR pszFormat, ...){
	std::va_list ArgList;

	va_start(ArgList, pszFormat);
	::MCF::__DebugMessageBoxA(::MCF::MBString().VFormat(pszFormat, ArgList), "Debug Alert", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
	va_end(ArgList);
}
inline void ALERT(LPCWSTR pwszMessage){
	::MCF::__DebugMessageBoxW(pwszMessage, L"Debug Alert", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
}
inline void ALERTF(LPCWSTR pwszFormat, ...){
	std::va_list ArgList;

	va_start(ArgList, pwszFormat);
	::MCF::__DebugMessageBoxW(::MCF::WCString().VFormat(pwszFormat, ArgList), L"Debug Alert", MB_ICONWARNING | MB_OK | MB_TASKMODAL);
	va_end(ArgList);
}

// ��������ַ�����
// �����ṩ ANSI �� Unicode �ַ��������ذ汾��ʹ��ʱ���ؼ��� xT()��
inline void TRACE(LPCSTR pszMessage){
	::OutputDebugStringA(::MCF::MBString(pszMessage) += '\n');
}
inline void TRACEF(LPCSTR pszFormat, ...){
	std::va_list ArgList;

	va_start(ArgList, pszFormat);
	::OutputDebugStringA(::MCF::MBString().VFormat(pszFormat, ArgList) += '\n');
	va_end(ArgList);
}
inline void TRACE(LPCWSTR pwszMessage){
	::OutputDebugStringW(::MCF::WCString(pwszMessage) += L'\n');
}
inline void TRACEF(LPCWSTR pwszFormat, ...){
	std::va_list ArgList;

	va_start(ArgList, pwszFormat);
	::OutputDebugStringW(::MCF::WCString().VFormat(pwszFormat, ArgList) += L'\n');
	va_end(ArgList);
}

#endif
