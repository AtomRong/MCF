// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include <map>
#include <memory>
#include "CRC32.hpp"
using namespace MCF;

// ��̬��Ա������
__declspec(naked) void __fastcall CRC32Hasher::xBuildTable(DWORD (* /* pardwTable */)[0x100], DWORD /* dwDivisor */){
	// �ο����ף�
	// http://www.relisoft.com/science/CrcOptim.html
	//   [ע1] ԭ���ṩ��������Ȩ�ϴ�λ��Ȩ��Сλ���򣩵� CRC ���㣬������ʹ�õ��Ƿ���Ȩ��Сλ��Ȩ�ϴ�λ���򣩡�
	//   [ע2] ԭ�ĵ� CRC �����ĳ�ʼֵ�� 0���˴��� -1 Ϊ��ʼֵ��������ɺ���а�λ����

/*
	for(DWORD i = 0; i < 256; ++i){
		DWORD dwReg = i;
		for(std::size_t j = 0; j < 8; ++j){
			const bool bLowerBit = (dwReg & 1) != 0;
			dwReg >>= 1;
			if(bLowerBit){
				dwReg ^= dwDivisor;
			}
		}
		(*pardwTable)[i] = dwReg;
	}
*/

#define		CALC_ELEMENT_STEP(reg, divisor, tmp)	\
	__asm shr reg, 1								\
	__asm sbb tmp, tmp								\
	__asm and tmp, divisor							\
	__asm xor reg, tmp

	__asm {
		push ebx
		push esi
		push edi

		xor esi, esi
		mov ebx, -100h
		xor eax, eax
		jmp xfill_loop
		align 16

	xfill_loop:
	}

	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)
	CALC_ELEMENT_STEP(eax, edx, edi)

	__asm {
		inc ebx
		mov dword ptr[ecx + ebx * 4 - 4h + 400h], eax
		mov eax, esi
		mov al, bl
		jnz xfill_loop

		pop edi
		pop esi
		pop ebx
		ret				// __fastcall
	}
}

// ���캯��������������
CRC32Hasher::CRC32Hasher(DWORD dwDivisor){
	xBuildTable(&xm_ardwTable, dwDivisor);
}

// �����Ǿ�̬��Ա������
void CRC32Hasher::xInitialize(void *pOutput){
	*(DWORD *)pOutput = 0xFFFFFFFF;
}
void CRC32Hasher::xUpdate(void *pOutput, const BYTE *pbyData, std::size_t uSize){
	DWORD dwReg = *(DWORD *)pOutput;

	const BYTE *pbyRead = pbyData;
	const BYTE *const pbyEnd = pbyRead + (std::ptrdiff_t)uSize;

	// ע������Ļ�������һ�� do ... while ѭ��������ȷ�����������������һ�������ͽ�����֮��������һ�� DWORD��
	if((std::size_t)(pbyEnd - pbyRead) >= sizeof(DWORD) * 2){
		const BYTE *const pbyFirstAlignedPoint = (const BYTE *)(((std::uintptr_t)(pbyRead + 3)) & -4);
		while(pbyRead < pbyFirstAlignedPoint){
			dwReg = xm_ardwTable[(std::size_t)((dwReg & 0xFF) ^ *(pbyRead++))] ^ (dwReg >> 8);
		}

		__asm {
			push ebx
			push esi
			push edi

			mov ebx, dword ptr[this]
			lea ebx, dword ptr[ebx].xm_ardwTable
			mov esi, dword ptr[pbyRead]
			mov edi, dword ptr[pbyEnd]
			and edi, -4
			mov edx, dword ptr[dwReg]
			jmp xcrc_loop
			align 16

		xcrc_loop:
			lodsd

			movzx ecx, al
			xor cl, dl
			shr edx, 8
			xor edx, dword ptr[ebx + ecx * 4]
			xor ecx, ecx
			movzx ecx, ah
			xor cl, dl
			shr edx, 8
			xor edx, dword ptr[ebx + ecx * 4]

			shr eax, 16

			movzx ecx, al
			mov cl, al
			xor cl, dl
			shr edx, 8
			xor edx, dword ptr[ebx + ecx * 4]
			movzx ecx, ah
			xor cl, dl
			shr edx, 8
			xor edx, dword ptr[ebx + ecx * 4]

			cmp esi, edi
			jnz xcrc_loop

			mov dword ptr[dwReg], edx
			mov dword ptr[pbyRead], esi

			pop edi
			pop esi
			pop ebx
		}
	}
	while(pbyRead < pbyEnd){
		dwReg = xm_ardwTable[(std::size_t)((dwReg & 0xFF) ^ *(pbyRead++))] ^ (dwReg >> 8);
	}

	*(DWORD *)pOutput = dwReg;
}
void CRC32Hasher::xFinalize(void *pOutput){
	*(DWORD *)pOutput = ~*(DWORD *)pOutput;
}

void CRC32Hasher::Transform(DWORD *pdwOutput, const BYTE *pbyData, std::size_t uSize, bool bIsEndOfStream){
	AbstractTransformerBaseClass::xTransform(pdwOutput, pbyData, uSize, bIsEndOfStream);
}
