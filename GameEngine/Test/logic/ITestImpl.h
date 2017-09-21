
#ifndef _TEST_LOGIC_ITEST_IMPL_H_
#define _TEST_LOGIC_ITEST_IMPL_H_


#include "../../Message/Test.h"

using namespace Message;
using namespace csg;

class CCli_ITest_t3_CallBack :public virtual csg_proxy::CCli_ITest_t3
{
public:
	virtual void response(bool b ,string os)override;

};

typedef CSmartPointShare<CCli_ITest_t3_CallBack> CCli_ITest_t3_CallBackPtr;


class ITestImpl:public virtual Message::ITest
{
public:
	virtual void t1_async(const CSessionPtr& ,CSrv_ITest_t1_Ptr&)override;

	virtual void t2_async(const CSessionPtr& ,Message::CSrv_ITest_t2_Ptr& ,int ,string)override;

	virtual void t3_async(const CSessionPtr& ,Message::CSrv_ITest_t3_Ptr& ,int ,string)override;
	

};




#endif