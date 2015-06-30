/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file ValueHolder.h
 * @author 122801281@qq.com
 * @date 2014/02/11 10:38:13
 * @brief 
 *  
 **/

#ifndef  __VALUEHOLDER_H_
#define  __VALUEHOLDER_H_

#include <string>
#include <map>

namespace smart_plugin_frame{

/*
 * 只保存原始类型和指针类型类型，不负责深层内存管理，复杂对象
 * 内存由策略负责释放，只存入指针
 */
class ValueHolder
{
public:
    ValueHolder(){};
    ~ValueHolder();
    bool init();
    template <typename T>
    bool set(const std::string & key, T value);
    template <typename T>
    bool get(const std::string & key, T& value);
    bool exist(const std::string & key);
    void clear();

private:
    typedef std::map<std::string, void *> ValueMapType;
    ValueMapType m_valueMap;
};

template <typename T>
bool ValueHolder::set(const std::string & key, T value)
{
    T * val = (T*)malloc(sizeof(T));
    if(NULL == val){
        log_error("malloc fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return false;
    }
    *val = value;
    m_valueMap[key] = val;
    return true;
//    return m_valueMap.set(key, val, 0) == bsl::HASH_INSERT_SEC;
}

template <typename T>
bool ValueHolder::get(const std::string & key, T & value)
{
    void * val;
    if(m_valueMap.find(key) == m_valueMap.end()){
        return false;
    }
    value = m_valueMap[key];
    return true;
}

}














#endif  //__VALUEHOLDER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
