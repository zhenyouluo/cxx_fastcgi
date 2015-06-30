/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file interface/Strategy.h
 * @author 122801281@qq.com
 * @date 2014/01/23 16:39:40
 * @brief 
 *  
 **/




#ifndef  STRATEGY_H_
#define  STRATEGY_H_

#include <string>
#include <vector>
#include "IStrategy.h"

namespace smart_plugin_frame{

class Strategy : public IStrategy
{
public:
    Strategy():m_valueHolder(NULL), m_strategyManager(NULL), m_needRun(true){}
    virtual ~Strategy(){}
    virtual const char * getStrategyName() = 0;
    virtual bool initDepends() = 0;
    virtual bool processInit(const char * basePath) = 0;
    virtual bool threadInit(const char * basePath) = 0;
    virtual bool reset() = 0;
    virtual bool run(Request* request, Response* response) = 0;
    virtual bool threadDestroy() = 0;
    virtual bool processDestroy() = 0;

    void setNeedRun(bool needRun){m_needRun = needRun;}
    bool getNeedRun(){return m_needRun;}
    std::vector<std::string> & getDepends(){return m_depends;}
    void setValueHolder(ValueHolder * valueHolder){m_valueHolder = valueHolder;}
    void setStrategyManager(StrategyManager * strategyManager){m_strategyManager = strategyManager;}

protected:
    template <typename T>
    bool setShareValue(const std::string & key, T value);
    template <typename T>
    bool getShareValue(const std::string & key, T& value);

    std::vector<std::string> m_depends;

private:
    ValueHolder * m_valueHolder;
    StrategyManager * m_strategyManager;
    bool m_needRun;
};

template <typename T>
bool Strategy::setShareValue(const std::string & key, T value)
{
    return m_valueHolder->set(key, value);
}

template <typename T>
bool Strategy::getShareValue(const std::string & key, T & value)
{
    return m_valueHolder->get(key, value);
}

}














#endif  //STRATEGY_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
