// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#ifndef MCF_STREAM_FILTERS_BASE64_HPP_
#define MCF_STREAM_FILTERS_BASE64_HPP_

#include "StreamFilterBase.hpp"

namespace MCF {

constexpr char BASE64_TABLE_MIME	[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
constexpr char BASE64_TABLE_URL		[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789*-";
constexpr char BASE64_TABLE_REGEXP	[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!-=";

class Base64Encoder : public StreamFilterBase {
private:
	unsigned char xm_abyTable[65];

	unsigned xm_uState;
	unsigned long xm_ulWord;

public:
	explicit Base64Encoder(const char *pchTable = BASE64_TABLE_MIME) noexcept;

protected:
	void xDoInit() override;
	void xDoUpdate(const void *pData, std::size_t uSize) override;
	void xDoFinalize() override;
};

class Base64Decoder : public StreamFilterBase {
private:
	signed char xm_aschTable[0x100];

	unsigned xm_uState;
	unsigned long xm_ulWord;

public:
	explicit Base64Decoder(const char *pchTable = BASE64_TABLE_MIME) noexcept;

protected:
	void xDoInit() override;
	void xDoUpdate(const void *pData, std::size_t uSize) override;
	void xDoFinalize() override;
};

}

#endif