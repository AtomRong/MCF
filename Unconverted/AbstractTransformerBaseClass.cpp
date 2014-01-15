// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "AbstractTransformerBaseClass.hpp"
using namespace MCF;

// ���캯��������������
AbstractTransformerBaseClass::AbstractTransformerBaseClass(){
	xm_bStreamEnded = true;
}
AbstractTransformerBaseClass::~AbstractTransformerBaseClass(){
}

// �����Ǿ�̬��Ա������
void AbstractTransformerBaseClass::xTransform(void *pOutput, const BYTE *pbyData, std::size_t uSize, bool bIsEndOfStream){
	if(xm_bStreamEnded){
		xInitialize(pOutput);

		xm_bStreamEnded = false;
	}

	xUpdate(pOutput, pbyData, uSize);

	if(bIsEndOfStream){
		xFinalize(pOutput);

		xm_bStreamEnded = true;
	}
}
