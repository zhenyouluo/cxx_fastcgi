/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file service_main.cpp
 * @author 122801281@qq.com
 * @date 2014/02/12 16:42:07
 * @brief 
 *  
 **/

#include <Pluma/Pluma.hpp>
#include <Pluma/DLibrary.hpp>
#include "IStrategy.h"
#include <iostream>
#include "log.h"
#include "StrategyManager.h"
#include <fcgi_stdio.h>
#include <unistd.h>
#include "para_data.h"

using namespace smart_plugin_frame;

struct FrameOption
{
    int workThreadNum;
    int logLevel;
    bool alsoLogTTY;
};

static const char * sg_basePath = "../conf";

static FrameOption sg_frameOption = {1, 16, 1};

void * threadFunc(void * arg)
{
    StrategyManager strategyMgr(sg_basePath);
    if(!strategyMgr.init()){
        log_error("init strategyManager fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return NULL;
    }

	FCGX_Request request;  
	FCGX_InitRequest(&request, 0, 0);
//	req->r = &request;
	int rc;
	for (;;)  
	{   
		rc = FCGX_Accept_r(&request);  
		Request* q = new Request();
		q->Init(request.in, request.envp);
		Response* p = new Response(request.out, request.err);
		if (rc < 0)  
			break;  
		if(!strategyMgr.execAllResetAndRun(q, p)){
			log_error("execAllResetAndRun fail [%s %d %s]",
					__FILE__, __LINE__, __func__);
			break;
		}

/*		Response* p = new Response(request.out, request.err);
		std::string kDefaultContentType("text/html; charset=utf-8");  
		p->header().set_status_code(cwf::HC_OK, "OK");
		p->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
		p->OutputHeader();
*/	//	p->WriteRaw("122801281@qq.com");
		std::string r = p->str(); 
		FCGX_PutStr(r.c_str(), r.size(), request.out);
		FCGX_Finish_r(&request);  
		delete q;
		delete p;
//		FCGX_PutStr("\r\nhello", strlen("hello"), request.out);
//		FCGX_FPrintF(request.out, "\r\ntest");
//		FCGX_FPrintF(request.out,  
//				"\r\n"
//				"test");  
	}
	
    return NULL;
}

bool loadConf(FrameOption * option)
{
    if(NULL == option){
        log_error("option should not NULL [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return false;
    }
    int val;
    option->workThreadNum = 1;
    return true;
}

int main(int argc, char * argv [])
{
    log_init(LL_TRACE, "session", "../log/smart_plugin_frame");
	FCGX_Init();
    if(!loadConf(&sg_frameOption)){
        log_error("load conf fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return -1;
    }


    StrategyManager strategyMgr(sg_basePath);
    if(!strategyMgr.init()){
        log_error("init strategyManager fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return -1;
    }
    static int MAX_THREAD_NUM = 30;

    if(sg_frameOption.workThreadNum > MAX_THREAD_NUM){
        log_warn("max thread num is %d, conf is %d, use %d",
                MAX_THREAD_NUM, sg_frameOption.workThreadNum, MAX_THREAD_NUM);
        sg_frameOption.workThreadNum = MAX_THREAD_NUM;
    }

    pthread_t thread[MAX_THREAD_NUM];
    
    int i;
    for(i = 0; i<sg_frameOption.workThreadNum; i++){
        if(pthread_create(&thread[i], NULL, threadFunc, NULL) != 0){
            log_error("pthread_create fail in func %s.", __func__);
            break;
        }
    }

    log_debug("%d/%d threads created in func %s.", i, sg_frameOption.workThreadNum, __func__);

    int j;
    for(j = 0; j < i; j++){
        if(pthread_join(thread[j], NULL) != 0){
            log_warn("pthread_join fail in func %s.", __func__);
            break;
        }
    }
    log_debug("%d/%d threads joined in func %s.", j, i, __func__);

    return 0;
}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
