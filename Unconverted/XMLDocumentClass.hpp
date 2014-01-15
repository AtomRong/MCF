// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#ifndef __MCF_XML_DOCUMENT_CLASS_HPP__
#define __MCF_XML_DOCUMENT_CLASS_HPP__

#include "StdMCF.hpp"
#include <string>

namespace MCF {
	class XMLDocumentClass {
	public:
		class XMLElementClass;
	private:
		XMLElementClass *xm_pRootElement;
	public:
		XMLDocumentClass();
		// bStringIsXML =
		//   true	pszXML_RootElementKey ��һ�� XML �ַ�����
		//			����������󣬹���һ���յ� XML �ĵ�����IsEmpty() ���� true����
		//   false	pszXML_RootElementKey �Ǹ�Ԫ�صļ�����
		XMLDocumentClass(const char *pszXML_RootElementKey, bool bStringIsXML);
		XMLDocumentClass(const XMLDocumentClass &src);
		XMLDocumentClass(XMLDocumentClass &&src);
		XMLDocumentClass &operator=(const XMLDocumentClass &src);
		XMLDocumentClass &operator=(XMLDocumentClass &&src);
		~XMLDocumentClass();
	public:
		// ���� nullptr ��ɹ������򷵻�ָ������ַ�λ�õ�ָ�롣
		const char *Parse(const char *pszXMLString);
		void Create(const char *pszRootElementKey);

		bool IsEmpty() const;
		void Clear();

		const XMLElementClass *GetRoot() const;
		XMLElementClass *GetRoot();

		const XMLElementClass *PathQuery(const char *pszPath) const;
		XMLElementClass *PathQuery(const char *pszPath);

		// �κ����ӽڵ�Ľڵ�� CData ����ʧ��
		// �� bToFormat = true�����ɵ��ַ����� LF ���С�TAB ������
		std::string Export(bool bToFormat = true) const;
	};

	typedef XMLDocumentClass::XMLElementClass XMLElementClass;
}

#endif
