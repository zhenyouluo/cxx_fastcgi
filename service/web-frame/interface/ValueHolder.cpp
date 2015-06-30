/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file ValueHolder.cpp
 * @author 122801281@qq.com
 * @date 2014/02/11 16:08:00
 * @brief 
 *  
 **/

#include "ValueHolder.h"

namespace smart_plugin_frame{

bool ValueHolder::init()
{
    static const int VH_BUCKET_SIZE = 1024;
    return true;
}

ValueHolder::~ValueHolder()
{
    ValueMapType::iterator it;
    for(it = m_valueMap.begin(); it != m_valueMap.end(); ++it){
        free(it->second);
    }
    m_valueMap.clear();
}

bool ValueHolder::exist(const std::string & key)
{
    return m_valueMap.find(key) != m_valueMap.end();
}

void ValueHolder::clear()
{
    ValueMapType::iterator it;
    for(it = m_valueMap.begin(); it != m_valueMap.end(); ++it){
        free(it->second);
    }
    m_valueMap.clear();
}

}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
