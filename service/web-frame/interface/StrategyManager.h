/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file StrategyManager.h
 * @author 122801281@qq.com
 * @date 2014/02/11 13:34:59
 * @brief 
 *  
 **/




#ifndef  __STRATEGYMANAGER_H_
#define  __STRATEGYMANAGER_H_

#include <string>
#include "ValueHolder.h"
#include "ScopeLocker.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/property_map/property_map.hpp"
#include "para_data.h"

namespace pluma{
    class Pluma;
}

namespace boost
{

enum vertex_strategy_t { vertex_strategy = 211 };
BOOST_INSTALL_PROPERTY(vertex, strategy);

enum edge_redundant_t { edge_redundant = 212 };
BOOST_INSTALL_PROPERTY(edge, redundant);

}

namespace smart_plugin_frame{

class IStrategy;
class IStrategyProvider;
class StrategyManager
{
public:
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, 
                        boost::property<boost::vertex_name_t, std::string,
                        boost::property<boost::vertex_strategy_t, IStrategy *> >,
                        boost::property<boost::edge_redundant_t, bool> > StgGraph;
    typedef boost::graph_traits<StgGraph>::edge_descriptor StgEdge;
    typedef boost::graph_traits<StgGraph>::vertex_descriptor StgVertex;

    StrategyManager(const std::string & baseConfPath):
        m_baseConfPath(baseConfPath){}
    ~StrategyManager();
    bool init();
    bool execAllResetAndRun(Request* request, Response* response);
    StgGraph & getStrategyGraph(){return m_stgGraph;}
    void markRedundantEdge();
    std::vector<IStrategy *> & getTopoStrategies(){ return m_topoStrategies;}
private:
    typedef std::map<std::string, IStrategy *> NameStrategyMap;

    bool addAllStrategy();
    static bool buildStrategyGraph(std::vector<IStrategy *> & strategies,
            StgGraph & stgGraph, NameStrategyMap & nameStrategyMap);
    static bool generateTopoOrder(StgGraph & stgGraph, 
            std::vector<IStrategy *> & topoStrategies);
    static bool checkCircle(const StgGraph & stgGraph, std::vector<StgEdge> & circleEdges);
    static bool markNeedRunByDepends(const std::string & strategyName, NameStrategyMap & nameStrategyMap);

    ValueHolder m_valueHolder;
    std::vector<IStrategy *> m_strategies;
    std::vector<IStrategy *> m_topoStrategies;
    std::map<StgVertex, IStrategy *> m_vertexStgMap;
    NameStrategyMap m_nameStrategyMap;
    StgGraph m_stgGraph;
    const std::string m_baseConfPath;
    static int sm_instanceNum;
    static MutexLock sm_processLock;
    static ::pluma::Pluma * sm_pluma;
    static std::vector<IStrategyProvider*> sm_strategyProviders;
    static std::string sm_pluginPath;
};

}














#endif  //__STRATEGYMANAGER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
