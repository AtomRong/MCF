// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

namespace __gnu_cxx {
	__attribute__((__weak__))
	void __freeres() noexcept;
}

extern "C" void __MCFCRT_libsupcxx_freeres() noexcept {
	if(__gnu_cxx::__freeres){
		__gnu_cxx::__freeres();
	}
}
