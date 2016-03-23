// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../StdMCF.hpp"
#include "_KernelObjectBase.hpp"
#include "../Core/Exception.hpp"
#include <winternl.h>
#include <ntdef.h>

extern "C" __attribute__((__dllimport__, __stdcall__))
NTSTATUS NtOpenDirectoryObject(HANDLE *pHandle, ACCESS_MASK dwDesiredAccess, const OBJECT_ATTRIBUTES *pObjectAttributes) noexcept;

namespace MCF {

namespace Impl_KernelObjectBase {
	Impl_UniqueNtHandle::UniqueNtHandle KernelObjectBase::Y_OpenBaseNamedObjectDirectory(std::uint32_t u32Flags){
		wchar_t awcNameBuffer[64];

		::UNICODE_STRING ustrName;
		if(u32Flags & kGlobal){
			static constexpr wchar_t kBaseNameObjects[] = L"\\BaseNamedObjects";

			ustrName.Length        = (USHORT)(sizeof(kBaseNameObjects) - sizeof(wchar_t));
			ustrName.MaximumLength = (USHORT)(sizeof(kBaseNameObjects));
			ustrName.Buffer        = (PWSTR)kBaseNameObjects;
		} else {
			const auto uLen = (unsigned)swprintf(awcNameBuffer, sizeof(awcNameBuffer) / sizeof(wchar_t), L"\\Sessions\\%lu\\BaseNamedObjects", (unsigned long)::WTSGetActiveConsoleSessionId());

			ustrName.Length        = (USHORT)(uLen * sizeof(wchar_t));
			ustrName.MaximumLength = (USHORT)(uLen * sizeof(wchar_t) + sizeof(wchar_t));
			ustrName.Buffer        = awcNameBuffer;
		}

		::OBJECT_ATTRIBUTES vObjectAttributes;
		InitializeObjectAttributes(&vObjectAttributes, &ustrName, 0, nullptr, nullptr);

		HANDLE hDirectory;
		const auto lStatus = ::NtOpenDirectoryObject(&hDirectory, 0x0F, &vObjectAttributes);
		if(!NT_SUCCESS(lStatus)){
			MCF_THROW(Exception, ::RtlNtStatusToDosError(lStatus), Rcntws::View(L"NtOpenDirectoryObject() 失败。"));
		}
		return Impl_UniqueNtHandle::UniqueNtHandle(hDirectory);
	}
}

}
