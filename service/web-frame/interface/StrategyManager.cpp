/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file StrategyManager.cpp
 * @author 122801281@qq.com
 * @date 2014/02/12 18:22:13
 * @brief 
 *  
 **/

#include <vector>
#include "StrategyManager.h"
#include "log.h"
#include "Pluma/Pluma.hpp"
#include "IStrategy.h"
#include "boost/graph/depth_first_search.hpp"
#include "boost/graph/topological_sort.hpp"

using namespace std;
using namespace boost;

namespace smart_plugin_frame{

int StrategyManager::sm_instanceNum = 0;
MutexLock StrategyManager::sm_processLock;
::pluma::Pluma * StrategyManager::sm_pluma = NULL;
std::vector<IStrategyProvider*> StrategyManager::sm_strategyProviders;
std::string StrategyManager::sm_pluginPath = "./plugins";


bool StrategyManager::init()
{
    if(!m_valueHolder.init()){
        log_error("init valueHolder fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return false;
    }

    {
        ScopeLocker locker(sm_processLock);
        if(sm_instanceNum == 0){
            if(!sm_pluma){
                sm_pluma = new ::pluma::Pluma();
                sm_pluma->acceptProviderType<IStrategyProvider>();
                sm_pluma->loadFromFolder(sm_pluginPath);
                sm_pluma->getProviders(sm_strategyProviders);
                
                //add by liangxiaohui for debug info
                std::vector<const std::string*> pluginNames;
                sm_pluma->getLoadedPlugins(pluginNames);
                std::vector<const std::string*>::const_iterator it;
                for(it=pluginNames.begin();it!= pluginNames.end();it++){
                    log_notice("load [Plugin: %s] success",(*it)->c_str());
                }
            }
            if(!addAllStrategy()){
                log_error("addAllStrategy fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            if(!buildStrategyGraph(m_strategies, m_stgGraph, m_nameStrategyMap)){
                log_error("buildStrategyGraph fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            vector<StgEdge> circleEdges;
            bool hasCircle = checkCircle(m_stgGraph, circleEdges);
            if(hasCircle){
                property_map<StgGraph, vertex_name_t>::type vertexNameMap = get(vertex_name, m_stgGraph);
                vector<StgEdge>::iterator it;
                StgVertex src, dest;
                StgEdge e;
                for(it = circleEdges.begin(); it != circleEdges.end(); ++it){
                    e = *it;
                    src = source(e, m_stgGraph);
                    dest = target(e, m_stgGraph);
                    log_error("Stategies has Circle Edge %s -> %s [%s %d %s]",
                            vertexNameMap[src].c_str(), vertexNameMap[dest].c_str(), __FILE__, __LINE__, __func__);
                }
                return false;
            }
            if(!generateTopoOrder(m_stgGraph, m_topoStrategies)){
                log_error("generateTopoOrder fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            vector<IStrategy *>::iterator it;
            IStrategy * strategy = NULL;
            for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
                strategy = *it;
                if(!strategy->processInit(m_baseConfPath.c_str())){
                    log_error("processInit fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                    return false;
                }
                if(!strategy->threadInit(m_baseConfPath.c_str())){
                    log_error("processInit fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                    return false;
                }
            }
            sm_instanceNum++;
        }else{
            if(!addAllStrategy()){
                log_error("addAllStrategy fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            if(!buildStrategyGraph(m_strategies, m_stgGraph, m_nameStrategyMap)){
                log_error("buildStrategyGraph fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            vector<StgEdge> circleEdges;
            bool hasCircle = checkCircle(m_stgGraph, circleEdges);
            if(hasCircle){
                property_map<StgGraph, vertex_name_t>::type vertexNameMap = get(vertex_name, m_stgGraph);
                vector<StgEdge>::iterator it;
                StgVertex src, dest;
                StgEdge e;
                for(it = circleEdges.begin(); it != circleEdges.end(); ++it){
                    e = *it;
                    src = source(e, m_stgGraph);
                    dest = target(e, m_stgGraph);
                    log_error("Stategies has Circle Edge %s -> %s [%s %d %s]",
                            vertexNameMap[src].c_str(), vertexNameMap[dest].c_str());
                }
                return false;
            }
            if(!generateTopoOrder(m_stgGraph, m_topoStrategies)){
                log_error("generateTopoOrder fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
            vector<IStrategy *>::iterator it;
            IStrategy * strategy = NULL;
            for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
                strategy = *it;
                if(!strategy->threadInit(m_baseConfPath.c_str())){
                    log_error("processInit fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                    return false;
                }
            }
            sm_instanceNum++;
        }
    }

    return true;
}

bool StrategyManager::addAllStrategy()
{
    std::vector<IStrategyProvider*>::iterator it;
    for (it = sm_strategyProviders.begin() ; it != sm_strategyProviders.end() ; ++it){
        IStrategy* strategy = (*it)->create();
        if(NULL == strategy){
            log_error("create strategy fail [%s %d %s]",
                    __FILE__, __LINE__, __func__);
            return false;
        }
        if(!strategy->initDepends()){
            log_error("init strategy %s depends fail [%s %d %s]",
                    strategy->getStrategyName(), __FILE__, __LINE__, __func__);
            return false;
        }
        strategy->setValueHolder(&m_valueHolder);
        strategy->setStrategyManager(this);
        m_strategies.push_back(strategy);
    }

    return true;
}

StrategyManager::~StrategyManager()
{
    {
        ScopeLocker locker(sm_processLock);
        if(sm_instanceNum == 1){
            vector<IStrategy *>::reverse_iterator rit;
            IStrategy * strategy = NULL;
            for(rit = m_topoStrategies.rbegin(); rit != m_topoStrategies.rend(); ++rit){
                strategy = *rit;
                if(!strategy->threadDestroy()){
                    log_error("threadDestroy fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                }
                if(!strategy->processDestroy()){
                    log_error("threadDestroy fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                }
            }
            vector<IStrategy *>::iterator it;
            for(it = m_strategies.begin(); it != m_strategies.end(); ++it){
                delete (*it);
            }
            if(sm_pluma){
                sm_pluma->unloadAll();
                delete sm_pluma;
                sm_pluma = NULL;
            }
            sm_instanceNum--;
        }else{
            vector<IStrategy *>::reverse_iterator rit;
            IStrategy * strategy = NULL;
            for(rit = m_topoStrategies.rbegin(); rit != m_topoStrategies.rend(); ++rit){
                strategy = *rit;
                if(!strategy->threadDestroy()){
                    log_error("threadDestroy fail [Stg: %s] [%s %d %s]",
                            strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                }
            }
            vector<IStrategy *>::iterator it;
            for(it = m_strategies.begin(); it != m_strategies.end(); ++it){
                delete (*it);
            }
            sm_instanceNum--;
        }
    }
}

static const char * getRunStrategies()
{
        return NULL;
}

bool StrategyManager::execAllResetAndRun(Request* request, Response* response)
{
    m_valueHolder.clear();
    const char * runStrategies = getRunStrategies();
    if(runStrategies){
        //run select strategies

        //set all strategies no need to run
        vector<IStrategy *>::iterator it;
        IStrategy * strategy = NULL;
        for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
            strategy = *it;
            strategy->setNeedRun(false);
        }
        //use depend to mark need run strategies
        char strategyName[256];
        size_t runStrategiesLen = strlen(runStrategies);
        bool meetChar = false;
        int j = 0;
        //loop to '\0' to deal last strategyName
        for(int i = 0; i<(runStrategiesLen + 1); i++){
            if(runStrategies[i] == ',' || runStrategies[i] == ' ' || runStrategies[i] == '\0'){
                if(meetChar){
                    strategyName[j] = '\0';
                    if(!markNeedRunByDepends(strategyName, m_nameStrategyMap)){
                        log_error("markNeedRunByDepends fail [%s %d %s]",
                                __FILE__, __LINE__, __func__);
                        return true;
                    }
                    meetChar = false;
                    j = 0;
                }else{
                    continue;
                }
            }else{
                if(meetChar){
                    if(j < (sizeof(strategyName) - 1)){
                        strategyName[j] = runStrategies[i];
                        ++j;
                    }else{
                        log_error("strategyName exceed, please check pack field run_strategies [%s %d %s]",
                                __FILE__, __LINE__, __func__);
                        return false;
                    }
                }else{
                    strategyName[j] = runStrategies[i];
                    ++j;
                    meetChar = true;
                }
            }
        }

        //run all need strategies
        for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
            strategy = *it;
            if(strategy->getNeedRun() && !strategy->reset()){
                log_error("reset fail [Stg: %s] [%s %d %s]",
                        strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                return false;
            }
        }
        for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
            strategy = *it;
            if(strategy->getNeedRun() && !strategy->run(request, response)){
                log_error("run fail [Stg: %s] [%s %d %s]",
                        strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                //if run fail continue to process next pack so return true
                return true;
            }
        }
    }else{
        //run all strategies
        vector<IStrategy *>::iterator it;
        IStrategy * strategy = NULL;
        for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
            strategy = *it;
            if(!strategy->reset()){
                log_error("reset fail [Stg: %s] [%s %d %s]",
                        strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                return false;
            }
        }
        for(it = m_topoStrategies.begin(); it != m_topoStrategies.end(); ++it){
            strategy = *it;
            if(!strategy->run(request, response)){
                return true;
            }
        }
    }
    
    return true;
}

bool StrategyManager::buildStrategyGraph(std::vector<IStrategy *> & strategies,
            StgGraph & stgGraph, NameStrategyMap & nameStrategyMap)
{
    typedef map<string, StgVertex> NameVertexMap;
    NameVertexMap nameVertexMap;

    vector<IStrategy *>::iterator it;
    IStrategy * strategy = NULL;
    StgVertex vertex;
    string strategyName;
    NameVertexMap::iterator nameVertexIt;
    bool inserted;
    property_map<StgGraph, vertex_name_t>::type vertexNameMap = get(vertex_name, stgGraph);
    property_map<StgGraph, vertex_strategy_t>::type vertexStrategyMap = get(vertex_strategy, stgGraph);
    //add all strategy vertex
    for(it = strategies.begin(); it != strategies.end(); ++it){
        strategy = *it;
        strategyName = strategy->getStrategyName();
        if(strategyName.empty()){
            log_error("getStrategyName fail [%s %d %s]",
                    __FILE__, __LINE__, __func__);
            return false;
        }
        tie(nameVertexIt, inserted) = nameVertexMap.insert(make_pair(strategyName, StgVertex()));
        if(inserted){
            vertex = add_vertex(stgGraph);
            vertexNameMap[vertex] = strategyName;
            vertexStrategyMap[vertex] = strategy;
            nameVertexIt->second = vertex;
            if(!(nameStrategyMap.insert(make_pair(strategyName, strategy)).second)){
                log_error("insert nameStrategyMap fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
                return false;
            }
        }
    }
    //add all depends
    StgVertex srcVertex, destVertex;
    for(it = strategies.begin(); it != strategies.end(); ++it){
        strategy = *it;
        strategyName = strategy->getStrategyName();
        if(strategyName.empty()){
            log_error("getStrategyName fail [%s %d %s]",
                    __FILE__, __LINE__, __func__);
            return false;
        }
        nameVertexIt = nameVertexMap.find(strategyName);
        if(nameVertexIt == nameVertexMap.end()){
            log_error("can't find Strategy %s [%s %d %s]",
                    strategyName.c_str(), __FILE__, __LINE__, __func__);
            return false;
        }
        destVertex = nameVertexIt->second;
        vector<string> & depends = strategy->getDepends();
        vector<string>::iterator dependStgNameIt;
        for(dependStgNameIt = depends.begin(); dependStgNameIt != depends.end(); ++dependStgNameIt){
            string & dependStgName = *dependStgNameIt;
            nameVertexIt = nameVertexMap.find(dependStgName);
            if(nameVertexIt == nameVertexMap.end()){
                log_error("can't find Depend Strategy %s for %s [%s %d %s]",
                        dependStgName.c_str(), strategy->getStrategyName(), __FILE__, __LINE__, __func__);
                return false;
            }
            srcVertex = nameVertexIt->second;
            if(!(add_edge(srcVertex, destVertex, stgGraph).second)){
                log_error("add_edge fail [%s %d %s]",
                        __FILE__, __LINE__, __func__);
            }
        }
    }
    return true;
}

template <typename Edge>
class CircleCheckVisitor : public default_dfs_visitor
{
public:
    CircleCheckVisitor(bool & hasCircle, vector<Edge> & circleEdges):m_hasCircle(hasCircle), m_circleEdges(circleEdges){}
    template < typename Graph >
    void back_edge(Edge e, const Graph & g) const {
        m_hasCircle = true;
        m_circleEdges.push_back(e);
    }
private:
    bool & m_hasCircle;
    vector<Edge> & m_circleEdges;
};

bool StrategyManager::checkCircle(const StgGraph & stgGraph, vector<StgEdge> & circleEdges)
{
    bool hasCircle = false;
    CircleCheckVisitor<StgEdge> circleCheckVisitor(hasCircle, circleEdges);
    depth_first_search(stgGraph, visitor(circleCheckVisitor));
    return hasCircle;
}
 
bool StrategyManager::generateTopoOrder(StgGraph & stgGraph, 
            std::vector<IStrategy *> & topoStrategies)
{
    std::vector<StgVertex> topoOrder(num_vertices(stgGraph));
    topological_sort(stgGraph, topoOrder.rbegin());
    std::vector<StgVertex>::iterator topoIter;
    property_map<StgGraph, vertex_strategy_t>::type vertexStrategyMap = get(vertex_strategy, stgGraph);
    topoStrategies.clear();
    for(topoIter = topoOrder.begin(); topoIter != topoOrder.end(); ++topoIter){
        topoStrategies.push_back(vertexStrategyMap[*topoIter]);
    }
    return true;
}
 
template<typename Vertex>
class ExistAnotherPathVisitor : public default_dfs_visitor
{
public:
    ExistAnotherPathVisitor(Vertex src, Vertex dest, bool & exist): m_src(src), m_dest(dest), m_exist(exist){}
    template < typename Edge, typename Graph >
    void examine_edge(Edge e, const Graph & g) const{
        Vertex src = source(e, g);
        Vertex dest = target(e, g);
        if(dest == m_dest && src != m_src){
            m_exist = true;
        }
    }
private:
    Vertex m_src;
    Vertex m_dest;
    bool & m_exist;
};
 
template <typename Graph>
class MarkRedundantVisitor : public default_dfs_visitor
{
public:
    MarkRedundantVisitor(Graph & g){m_nameMap = get(vertex_name, g);m_redundantMap = get(edge_redundant, g);}
    template < typename Edge>
    void examine_edge(Edge e, const Graph & g) const{
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        Vertex src = source(e, g);
        Vertex dest = target(e, g);
        bool exist = false;
        ExistAnotherPathVisitor<Vertex> vis(src, dest, exist);
        vector<default_color_type> colorMap(num_vertices(g));
        depth_first_visit(g, src, vis, &colorMap[0]);
        if(exist){
            m_redundantMap[edge(src, dest, g).first] = true;
        }
    }
private:
    typename property_map<Graph, vertex_name_t>::type m_nameMap;
    typename property_map<Graph, edge_redundant_t>::type m_redundantMap;
};
 
void StrategyManager::markRedundantEdge()
{
    MarkRedundantVisitor<StgGraph> markRedundVisitor(m_stgGraph);
    depth_first_search(m_stgGraph, visitor(markRedundVisitor));
}

bool StrategyManager::markNeedRunByDepends(const string & strategyName, NameStrategyMap & nameStrategyMap)
{
    NameStrategyMap::iterator nameStrategyIt;
    nameStrategyIt = nameStrategyMap.find(strategyName);
    if(nameStrategyIt == nameStrategyMap.end()){
        log_warn("can't find Strategy %s [%s %d %s]",
                strategyName.c_str(), __FILE__, __LINE__, __func__);
        return false;
    }
    IStrategy * strategy = nameStrategyIt->second;
    strategy->setNeedRun(true);
    vector<string> & depends = strategy->getDepends();
    vector<string>::iterator it;
    for(it = depends.begin(); it != depends.end(); ++it){
        if(!markNeedRunByDepends(*it, nameStrategyMap)){
            log_warn("markNeedRunByDepends fail [%s %d %s]",
                    __FILE__, __LINE__, __func__);
            return false;
        }
    }
    return true;
}

}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
