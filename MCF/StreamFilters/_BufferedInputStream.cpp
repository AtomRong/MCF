// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../StdMCF.hpp"
#include "_BufferedInputStream.hpp"

namespace MCF {

namespace Impl_BufferedInputStream {
	namespace {
		enum : std::size_t {
			 kStepSize = 4096,
		};

		void PopulateTempBuffer(StreamBuffer &vBuffer, Vector<unsigned char> &vecBackBuffer, AbstractInputStream *pStream, std::size_t uExpected){
			bool bNoMoreAvail = false;
			for(;;){
				if(!vecBackBuffer.IsEmpty()){
					vBuffer.Put(vecBackBuffer.GetData(), vecBackBuffer.GetSize());
					vecBackBuffer.Clear();
				}
				if(bNoMoreAvail || (vBuffer.GetSize() >= uExpected)){
					break;
				}

				std::size_t uBytesRead;
				const auto pbyStepBuffer = vecBackBuffer.ResizeMore(kStepSize);
				try {
					uBytesRead = pStream->Get(pbyStepBuffer, kStepSize);
				} catch(...){
					vecBackBuffer.Pop(kStepSize);
					throw;
				}
				vecBackBuffer.Pop(kStepSize - uBytesRead);

				if(uBytesRead < kStepSize){
					bNoMoreAvail = true;
				}
			}
		}
	}

	BufferedInputStream::~BufferedInputStream(){
	}

	int BufferedInputStream::Peek(){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), 1);
		return x_vBuffer.Peek();
	}
	int BufferedInputStream::Get(){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), 1);
		return x_vBuffer.Get();
	}
	bool BufferedInputStream::Discard(){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), 1);
		return x_vBuffer.Discard();
	}

	std::size_t BufferedInputStream::Peek(void *pData, std::size_t uSize){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), uSize);
		return x_vBuffer.Peek(pData, uSize);
	}
	std::size_t BufferedInputStream::Get(void *pData, std::size_t uSize){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), uSize);
		return x_vBuffer.Get(pData, uSize);
	}
	std::size_t BufferedInputStream::Discard(std::size_t uSize){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), uSize);
		return x_vBuffer.Discard(uSize);
	}

	StreamBuffer BufferedInputStream::CutOff(std::size_t uSize){
		PopulateTempBuffer(x_vBuffer, x_vecBackBuffer, x_pUnderlyingStream.Get(), uSize);
		return x_vBuffer.CutOff(uSize);
	}
}

}