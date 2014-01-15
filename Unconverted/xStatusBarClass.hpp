// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_X_STATUS_BAR_CLASS_HPP__
#define __MCF_X_STATUS_BAR_CLASS_HPP__

#include "StdMCF.hpp"
#include <vector>
#include <CommCtrl.h>
#include "WindowBaseClass.hpp"
#include "StandaloneWindowClass.hpp"

namespace MCF {
	class StandaloneWindowClass::xStatusBarClass : public WindowBaseClass {
	private:
		StandaloneWindowClass *const xm_pOwner;
	public:
		xStatusBarClass(StandaloneWindowClass *pOwner);
		~xStatusBarClass();
	public:
		void Update();
	};
}

#endif
