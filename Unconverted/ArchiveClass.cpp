// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "ArchiveClass.hpp"
using namespace MCF;

// MCF ��׼�ַ��������л�/�����л�����ʵ������̬��Ա�������塣
void xArchiveBaseClass::xArchiver<MBString>::Serialize(OutArchiveClass &dst, const MBString &src) throw() {
	DWORD dwStrLen = (DWORD)src.GetLength();
	dst <<dwStrLen;
	dst.InsertArray(src.GetBuffer(), dwStrLen);
}
void xArchiveBaseClass::xArchiver<MBString>::Unserialize(MBString &dst, InArchiveClass &src){
	DWORD dwStrLen;
	src >>dwStrLen;
	dst.Resize((std::size_t)dwStrLen, false);
	src.ExtractArray(dst.GetBuffer(), dwStrLen);
}

void xArchiveBaseClass::xArchiver<WCString>::Serialize(OutArchiveClass &dst, const WCString &src) throw() {
	DWORD dwStrLen = (DWORD)src.GetLength();
	dst <<dwStrLen;
	dst.InsertArray(src.GetBuffer(), dwStrLen);
}
void xArchiveBaseClass::xArchiver<WCString>::Unserialize(WCString &dst, InArchiveClass &src){
	DWORD dwStrLen;
	src >>dwStrLen;
	dst.Resize((std::size_t)dwStrLen, false);
	src.ExtractArray(dst.GetBuffer(), dwStrLen);
}

void xArchiveBaseClass::xArchiver<UTF8String>::Serialize(OutArchiveClass &dst, const UTF8String &src) throw() {
	DWORD dwStrLen = (DWORD)src.GetLength();
	dst <<dwStrLen;
	dst.InsertArray(src.GetBuffer(), dwStrLen);
}
void xArchiveBaseClass::xArchiver<UTF8String>::Unserialize(UTF8String &dst, InArchiveClass &src){
	DWORD dwStrLen;
	src >>dwStrLen;
	dst.Resize((std::size_t)dwStrLen, false);
	src.ExtractArray(dst.GetBuffer(), dwStrLen);
}
