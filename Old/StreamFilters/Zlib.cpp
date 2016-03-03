// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../StdMCF.hpp"
#include "Zlib.hpp"
#include "../Core/Exception.hpp"
#include "../Utilities/MinMax.hpp"

#define ZLIB_CONST
#define Z_PREFIX
#include "../../External/zlib/zlib.h"

namespace MCF {

namespace {
	constexpr std::size_t kStepSize = 0x4000;

	unsigned long ZlibErrorToWin32Error(int nZlibError) noexcept {
		switch(nZlibError){
		case Z_OK:
			return ERROR_SUCCESS;

		case Z_STREAM_END:
			return ERROR_HANDLE_EOF;

		case Z_NEED_DICT:
			return ERROR_INVALID_PARAMETER;

		case Z_ERRNO:
			return ERROR_OPEN_FAILED;

		case Z_STREAM_ERROR:
			return ERROR_INVALID_PARAMETER;

		case Z_DATA_ERROR:
			return ERROR_INVALID_DATA;

		case Z_MEM_ERROR:
			return ERROR_NOT_ENOUGH_MEMORY;

		case Z_BUF_ERROR:
			return ERROR_SUCCESS;

		case Z_VERSION_ERROR:
			return ERROR_NOT_SUPPORTED;

		default:
			return ERROR_INVALID_FUNCTION;
		}
	}
}

class ZlibEncoder::X_Delegate {
private:
	ZlibEncoder &x_vOwner;

	::z_stream x_vStream;

public:
	X_Delegate(ZlibEncoder &vOwner, bool bRaw, unsigned uLevel)
		: x_vOwner(vOwner)
	{
		x_vStream.zalloc = nullptr;
		x_vStream.zfree  = nullptr;
		x_vStream.opaque = nullptr;

		const auto nError = ::deflateInit2(&x_vStream, (int)uLevel, Z_DEFLATED, bRaw ? -15 : 15, 9, Z_DEFAULT_STRATEGY);
		if(nError != Z_OK){
			DEBUG_THROW(ZlibError, nError, "deflateInit2"_rcs);
		}
	}
	~X_Delegate(){
		::deflateEnd(&x_vStream);
	}

public:
	void Init(){
		const auto nError = ::deflateReset(&x_vStream);
		if(nError != Z_OK){
			DEBUG_THROW(ZlibError, nError, "deflateReset"_rcs);
		}
	}
	void Update(const void *pData, std::size_t uSize){
		unsigned char abyTemp[kStepSize];
		x_vStream.next_out = abyTemp;
		x_vStream.avail_out = sizeof(abyTemp);

		auto pbyRead = (const unsigned char *)pData;
		std::size_t uProcessed = 0;
		while(uProcessed < uSize){
			const auto uToProcess = Min(uSize - uProcessed, kStepSize);

			x_vStream.next_in = pbyRead;
			x_vStream.avail_in = (unsigned)uToProcess;
			do {
				const auto nError = ::deflate(&x_vStream, Z_NO_FLUSH);
				if(nError == Z_STREAM_END){
					break;
				}
				if(nError != Z_OK){
					DEBUG_THROW(ZlibError, nError, "deflate"_rcs);
				}
				if(x_vStream.avail_out == 0){
					x_vOwner.X_Output(abyTemp, sizeof(abyTemp));

					x_vStream.next_out = abyTemp;
					x_vStream.avail_out = sizeof(abyTemp);
				}
			} while(x_vStream.avail_in != 0);

			pbyRead += uToProcess;
			uProcessed += uToProcess;
		}
		if(x_vStream.avail_out != 0){
			x_vOwner.X_Output(abyTemp, sizeof(abyTemp) - x_vStream.avail_out);
		}
	}
	void Finalize(){
		unsigned char abyTemp[kStepSize];
		x_vStream.next_out = abyTemp;
		x_vStream.avail_out = sizeof(abyTemp);

		x_vStream.next_in = nullptr;
		x_vStream.avail_in = 0;
		for(;;){
			const auto nError = ::deflate(&x_vStream, Z_FINISH);
			if(nError == Z_STREAM_END){
				break;
			}
			if(nError != Z_OK){
				DEBUG_THROW(ZlibError, nError, "deflate"_rcs);
			}
			if(x_vStream.avail_out == 0){
				x_vOwner.X_Output(abyTemp, sizeof(abyTemp));

				x_vStream.next_out = abyTemp;
				x_vStream.avail_out = sizeof(abyTemp);
			}
		}
		if(x_vStream.avail_out != 0){
			x_vOwner.X_Output(abyTemp, sizeof(abyTemp) - x_vStream.avail_out);
		}
	}
};

class ZlibDecoder::X_Delegate {
private:
	ZlibDecoder &x_vOwner;

	::z_stream x_vStream;

public:
	X_Delegate(ZlibDecoder &vOwner, bool bRaw)
		: x_vOwner(vOwner)
	{
		x_vStream.zalloc   = nullptr;
		x_vStream.zfree    = nullptr;
		x_vStream.opaque   = nullptr;

		x_vStream.next_in  = nullptr;
		x_vStream.avail_in = 0;

		const auto nError = ::inflateInit2(&x_vStream, bRaw ? -15 : 15);
		if(nError != Z_OK){
			DEBUG_THROW(ZlibError, nError, "inflateInit2"_rcs);
		}
	}
	~X_Delegate(){
		::inflateEnd(&x_vStream);
	}

public:
	void Init(){
		const auto nError = ::inflateReset(&x_vStream);
		if(nError != Z_OK){
			DEBUG_THROW(ZlibError, nError, "inflateReset"_rcs);
		}
	}
	void Update(const void *pData, std::size_t uSize){
		unsigned char abyTemp[kStepSize];
		x_vStream.next_out = abyTemp;
		x_vStream.avail_out = sizeof(abyTemp);

		auto pbyRead = (const unsigned char *)pData;
		std::size_t uProcessed = 0;
		while(uProcessed < uSize){
			const auto uToProcess = Min(uSize - uProcessed, kStepSize);

			x_vStream.next_in = pbyRead;
			x_vStream.avail_in = (unsigned)uToProcess;
			do {
				const auto nError = ::inflate(&x_vStream, Z_NO_FLUSH);
				if(nError == Z_STREAM_END){
					break;
				}
				if(nError != Z_OK){
					DEBUG_THROW(ZlibError, nError, "inflate"_rcs);
				}
				if(x_vStream.avail_out == 0){
					x_vOwner.X_Output(abyTemp, sizeof(abyTemp));

					x_vStream.next_out = abyTemp;
					x_vStream.avail_out = sizeof(abyTemp);
				}
			} while(x_vStream.avail_in != 0);

			pbyRead += uToProcess;
			uProcessed += uToProcess;
		}
		if(x_vStream.avail_out != 0){
			x_vOwner.X_Output(abyTemp, sizeof(abyTemp) - x_vStream.avail_out);
		}
	}
	void Finalize(){
		unsigned char abyTemp[kStepSize];
		x_vStream.next_out = abyTemp;
		x_vStream.avail_out = sizeof(abyTemp);

		x_vStream.next_in = nullptr;
		x_vStream.avail_in = 0;
		for(;;){
			const auto nError = ::inflate(&x_vStream, Z_FINISH);
			if(nError == Z_STREAM_END){
				break;
			}
			if(nError != Z_OK){
				DEBUG_THROW(ZlibError, nError, "inflate"_rcs);
			}
			if(x_vStream.avail_out == 0){
				x_vOwner.X_Output(abyTemp, sizeof(abyTemp));

				x_vStream.next_out = abyTemp;
				x_vStream.avail_out = sizeof(abyTemp);
			}
		}
		if(x_vStream.avail_out != 0){
			x_vOwner.X_Output(abyTemp, sizeof(abyTemp) - x_vStream.avail_out);
		}
	}
};

// ========== ZlibEncoder ==========
// 静态成员函数。
ZlibEncoder::ZlibEncoder(bool bRaw, unsigned uLevel) noexcept
	: x_bRaw(bRaw), x_uLevel(uLevel)
{
}
ZlibEncoder::~ZlibEncoder(){
}

void ZlibEncoder::X_DoInit(){
	if(!x_pDelegate){
		x_pDelegate.Reset(new X_Delegate(*this, x_bRaw, x_uLevel));
	}
	x_pDelegate->Init();
}
void ZlibEncoder::X_DoUpdate(const void *pData, std::size_t uSize){
	x_pDelegate->Update(pData, uSize);
}
void ZlibEncoder::X_DoFinalize(){
	x_pDelegate->Finalize();
}

// ========== ZlibDecoder ==========
// 静态成员函数。
ZlibDecoder::ZlibDecoder(bool bRaw) noexcept
	: x_bRaw(bRaw)
{
}
ZlibDecoder::~ZlibDecoder(){
}

void ZlibDecoder::X_DoInit(){
	if(!x_pDelegate){
		x_pDelegate.Reset(new X_Delegate(*this, x_bRaw));
	}
	x_pDelegate->Init();
}
void ZlibDecoder::X_DoUpdate(const void *pData, std::size_t uSize){
	x_pDelegate->Update(pData, uSize);
}
void ZlibDecoder::X_DoFinalize(){
	x_pDelegate->Finalize();
}

// ========== ZlibError ==========
ZlibError::ZlibError(const char *pszFile, unsigned long ulLine, long lZlibError, RefCountingNtmbs rcsFunction) noexcept
	: Exception(pszFile, ulLine, ZlibErrorToWin32Error(lZlibError), std::move(rcsFunction))
	, x_lZlibError(lZlibError)
{
}
ZlibError::~ZlibError(){
}

}