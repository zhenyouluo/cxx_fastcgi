/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file IStrategy.h
 * @author 122801281@qq.com
 * @date 2014/01/23 10:45:06
 * @brief 
 *  
 **/




#ifndef  __ISTRATEGY_H_
#define  __ISTRATEGY_H_

#include "Pluma/Pluma.hpp"
#include "ValueHolder.h"
#include "para_data.h"

namespace smart_plugin_frame{

class StrategyManager;

class IStrategy
{
public:
    virtual ~IStrategy(){};
    virtual const char * getStrategyName() = 0;
    virtual bool initDepends() = 0;
    virtual bool processInit(const char * basePath) = 0;
    virtual bool threadInit(const char * basePath) = 0;
    virtual bool reset() = 0;
    virtual bool run(Request* request, Response* response) = 0;
    virtual bool threadDestroy() = 0;
    virtual bool processDestroy() = 0;
    virtual std::vector<std::string> & getDepends() = 0;
    virtual void setValueHolder(ValueHolder * valueHolder) = 0;
    virtual void setStrategyManager(StrategyManager * strategyManager) = 0;
    virtual void setNeedRun(bool needRun) = 0;
    virtual bool getNeedRun() = 0;
};

PLUMA_PROVIDER_HEADER(IStrategy);

#define FRAME_DECLARE_STRATEGY_NAME(CLASSNAME) \
    virtual const char * getStrategyName(){return #CLASSNAME;}

};














#endif  //__ISTRATEGY_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
