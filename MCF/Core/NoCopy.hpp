// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_NO_COPY_HPP__
#define __MCF_NO_COPY_HPP__

namespace MCF {

namespace __MCF {
	struct NonCopyable {
		NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(NonCopyable &&) = delete;
		void operator=(NonCopyable &&) = delete;
	};
}

}

#define NO_COPY		private ::MCF::__MCF::NonCopyable

#endif
