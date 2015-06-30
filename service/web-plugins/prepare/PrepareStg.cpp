/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file PrepareStg.cpp
 * @author 122801281@qq.com
 * @date 2014/06/27 16:14:38
 * @brief 
 *  
 **/
#include "PrepareStg.h"
#include "log.h"

using namespace cwf;

namespace smart_plugin_frame {

	FrameWork* PrepareStg::fw = new FrameWork();

	bool PrepareStg::initDepends()
	{
		return true;
	}

	bool PrepareStg::processInit(const char * basePath)
	{
		FrameWork::RegisterAction(new GuideAction);
		FrameWork::RegisterAction(new cwf::TestAction);
		fw->InitHost(true);
		return true;
	}
	bool PrepareStg::threadInit(const char * basePath)
	{
		return true;
	}
	bool PrepareStg::reset()
	{
		return true;
	}
	bool PrepareStg::run(Request* request, Response* response)
	{
		log_error("PrepareStg::run fail [%s %d %s]",
				__FILE__, __LINE__, __func__);
		HttpStatusCode rc = fw->Process(request, response);
		return true;
	}

	bool PrepareStg::threadDestroy()
	{
		return true;
	}

	bool PrepareStg::processDestroy()
	{
		return true;
	}

}






/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
