// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2015, LH_Mouse. All wrongs reserved.

#ifndef MCFBUILD_LOCALIZATION_HPP_
#define MCFBUILD_LOCALIZATION_HPP_

#include <MCF/Core/String.hpp>

namespace MCFBuild {

struct Localization {
	static MCF::WideString &Get(MCF::WideString &wcsAppendsTo, const char *pszKey);

private:
	Localization() = delete;
};

}

#endif
