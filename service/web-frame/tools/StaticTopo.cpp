/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file StaticTopo.cpp
 * @author 122801281@qq.com
 * @date 2014/03/18 11:12:31
 * @brief 
 *  
 **/

#include <string>
#include <vector>
#include "IStrategy.h"
#include "Pluma/Pluma.hpp"
#include "StrategyManager.h"

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/depth_first_search.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/graph/topological_sort.hpp"
#include "log.h"

using namespace boost;
using namespace smart_plugin_frame;
using namespace std;

static bool sg_outputFullGraph = false;
ul_logstat_t sg_logstat = {4, 0, 0};

template<typename Graph>
class PrintVisitor : public default_dfs_visitor
{
public:
    PrintVisitor(Graph & g, bool notPrintRedundant):m_notPrintRedundant(notPrintRedundant)
    {
        m_nameMap = get(vertex_name, g);
        m_redundantMap = get(edge_redundant, g);
    }
    template < typename Edge >
    void examine_edge(Edge e, const Graph & g) const{
        typename graph_traits<Graph>::vertex_descriptor src = source(e, g);
        typename graph_traits<Graph>::vertex_descriptor dest = target(e, g);
        if(m_notPrintRedundant){
            if(!(m_redundantMap[edge(src, dest, g).first])){
                printf("\t%s -> %s;\n", m_nameMap[src].c_str(), m_nameMap[dest].c_str());
            }
        }else{
                printf("\t%s -> %s;\n", m_nameMap[src].c_str(), m_nameMap[dest].c_str());
        }
    }
private:
    typename property_map<Graph, vertex_name_t>::type m_nameMap;
    typename property_map<Graph, edge_redundant_t>::type m_redundantMap;
    bool m_notPrintRedundant;
};

static void printUsage(const char * processName)
{
    fprintf(stderr, "usage: %s [-f]\n", processName);
    fprintf(stderr, "\t-f 表示按策略配置绘制全部依赖图，如不指定，默认将等价去除冗余的边，使图看起来更清晰\n");
}

int main(int argc, char ** argv)
{
    int opt_ch;
    while((opt_ch = getopt(argc, argv, "fh")) != -1){
        switch(opt_ch){
            case 'f':
                sg_outputFullGraph = true;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
                break;
            default:
                break;
        }
    }
    
    if(ul_openlog("../log", "static-topo", &sg_logstat, 1024) != 0){
        ul_writelog(UL_LOG_FATAL, "open log fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return -1;
    }

    StrategyManager strategyMgr("../conf");
    if(!strategyMgr.init()){
        ul_writelog(UL_LOG_FATAL, "init strategyManager fail [%s %d %s]",
                __FILE__, __LINE__, __func__);
        return -1;
    }

    StrategyManager::StgGraph & stgGraph = strategyMgr.getStrategyGraph();

    printf("digraph StrategyDepends {\n");
    if(sg_outputFullGraph){
        PrintVisitor<StrategyManager::StgGraph> vis(stgGraph, false);
        depth_first_search(stgGraph, visitor(vis));
    }else{
        strategyMgr.markRedundantEdge();
        PrintVisitor<StrategyManager::StgGraph> vis(stgGraph, true);
        depth_first_search(stgGraph, visitor(vis));
    }
    printf("}\n");
    /*
    std::vector<graph_traits<StgGraph>::vertex_descriptor> topoOrder(num_vertices(stgGraph));
    topological_sort(stgGraph, topoOrder.rbegin());
    std::vector<graph_traits<StgGraph>::vertex_descriptor>::iterator topoIter;
    for(topoIter = topoOrder.begin(); topoIter != topoOrder.end(); ++topoIter){
        printf("%s -> ", nameMap[*topoIter].c_str());
    }
    */
    ul_closelog(0);
    return 0;
}


























/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
