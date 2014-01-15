// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "StdMCF.hpp"
#include "xPooledThreadClass.hpp"
using namespace MCF;

// ���캯��������������
ThreadPoolClass::xPooledThreadClass::xPooledThreadClass(ThreadPoolClass *pOwner) :
	xm_pOwner(pOwner)
{
}
ThreadPoolClass::xPooledThreadClass::~xPooledThreadClass(){
}

// �����Ǿ�̬��Ա������
int ThreadPoolClass::xPooledThreadClass::xThreadProc(){
	for(;;){
		xm_pOwner->xm_semJobsAvailable.Wait();
		if(xm_pOwner->xm_bTerminateNow){
			break;
		}

		std::size_t uJobID = 0;
		std::function<void()> JobProc;

		xm_pOwner->xm_csLock.Enter();
			if(!xm_pOwner->xm_quePendingJobs.empty()){
				JobProc = std::move(xm_pOwner->xm_quePendingJobs.front().first);
				xm_pOwner->xm_quePendingJobs.pop();
				uJobID = ++xm_pOwner->xm_uFirstPendingJobID;
			}
		xm_pOwner->xm_csLock.Leave();

		if(JobProc){
			JobProc();

			xm_pOwner->xm_csLock.Enter();
				const auto iterEvents = xm_pOwner->xm_mapDoneEvents.find(uJobID);

				ASSERT(iterEvents != xm_pOwner->xm_mapDoneEvents.end());

				::SetEvent(iterEvents->second);
				xm_pOwner->xm_mapDoneEvents.erase(iterEvents);
			xm_pOwner->xm_csLock.Leave();
		}
	}
	return 0;
};
