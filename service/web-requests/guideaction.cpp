/*==================================
* Copyright (C) 2014 All rights reserved.
* 
* file：guideaction.cpp
* author：122801281@qq.com
* date：2014-11-28
* description：
*
================================================================*/
#include "guideaction.h"
#include "cwf/stream.h"

namespace cwf {
	int GuideAction::counter_ = 0;
	GuideAction::GuideAction(){
	}
	bool GuideAction::Match(const std::string& url) const {
		return url.compare("/guideaction") == 0;
	}
	HttpStatusCode GuideAction::Process(Request* req, Response* res) {
		std::ostringstream oss;
		oss << "GuideAction::Process() counter:" << (++counter_);
		SetHeader(res);
		res->WriteRaw("guideaction");
		return cwf::HC_OK;
	}

	void GuideAction::SetHeader(Response* res) {
		static const std::string kDefaultContentType("text/html; charset=utf-8");
		res->header().set_status_code(cwf::HC_OK, "OK");
		res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
		res->OutputHeader();
	}

}

