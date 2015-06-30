/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file PrepareStg.h
 * @author 122801281@qq.com
 * @date 2014/06/27 16:06:24
 * @brief 
 *  
 **/




#ifndef  __PREPARE_STG_H_
#define  __PREPARE_STG_H_

#include "Strategy.h"
#include "cwf/frame.h"
#include "cwf/http.h"
#include "guideaction.h"
#include "testaction.h"
#define MAX_PATH_LEN 2048

using namespace cwf;
namespace smart_plugin_frame {

class PrepareStg :  public Strategy
{
public:
    PrepareStg(){};
    virtual const char* getStrategyName(){return "PrepareStg";}
    virtual bool initDepends();
    virtual bool processInit(const char* basePath);
    virtual bool threadInit(const char* basePath);
    virtual bool reset();
    virtual bool run(Request* request, Response* response);
    virtual bool threadDestroy();
    virtual bool processDestroy();
private:
	static FrameWork* fw;
};

PLUMA_INHERIT_PROVIDER(PrepareStg, IStrategy);

}














#endif  //__PAGEEXTRACTSTG_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
