// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "xBase64CodecBase.hpp"
using namespace MCF;

// ��̬��Ա������
const xBase64CodecBase::PARAMS	xBase64CodecBase::B64P_MIME		= { '+', '/', '=' };
const xBase64CodecBase::PARAMS	xBase64CodecBase::B64P_URL		= { '*', '-',  0  };
const xBase64CodecBase::PARAMS	xBase64CodecBase::B64P_REGEXP	= { '!', '-', '=' };
