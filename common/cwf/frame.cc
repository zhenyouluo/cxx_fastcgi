/*==================================
* Copyright (C) 2014 All rights reserved.
* 
* file：frame.cc
* author：122801281@qq.com
* date：2014-11-28
* description：
*
================================================================*/
#include "cwf/frame.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>

#include <iostream> // TODO: remove
#include <set>

#include "cwf/action.h"

/*namespace xce {
extern cwf::User* Authorize(cwf::Request* q);
}*/

namespace cwf {

	static const std::string kDefaultContentType("text/html; charset=utf-8");

	void GenerateCommonHeader(Header* header, HttpStatusCode status_code
			, std::string const & message, std::string const& content_type = "") {
		// HTTP/1.x 200 OK
		header->set_status_code(status_code, message);
		if (!content_type.empty())
			header->Add(HH_CONTENT_TYPE, content_type);
	}

	bool FrameWork::LoadConfig(const std::string& filename) {
		// TODO: 现在没有想到任何用途，可以删除
		return true;
	}

	bool FrameWork::InitHost(bool load_action) {
		if (default_actions_)
			host_action_.assign(default_actions_->begin(), default_actions_->end());
		return true;
	}

	BaseAction* FrameWork::Find(std::string const& url) const {
		for (ActionListType::const_iterator i=host_action_.begin();
				i!= host_action_.end(); ++i) {
			BaseAction* ha = *i;
			if (ha->Match(url))
				return ha;
		}
		return 0;
	}

	//XAR_IMPL(cwferr);
	//XAR_IMPL(cwfall);
	//XAR_IMPL(prcGT100);
	HttpStatusCode FrameWork::Process(Request* request, Response* response) {
		HttpStatusCode rc;
		BaseAction* a = Find(request->url());
		if (!a) {
			ResponseError(HC_NOT_FOUND, "Not Found", response);
			return HC_NOT_FOUND;
		}

		//XAR_INC(cwfall);
		rc = a->Process(request, response);

//		if (pt.wall_clock() > 100)
//			XAR_INC(prcGT100);

		if (HC_OK != rc) {
		//	XAR_INC(cwferr);
			ResponseError(rc, "Service", response);
			return rc;
		}
		return rc;
	}

	FrameWork::ActionListType * FrameWork::default_actions_ = 0;

	void FrameWork::RegisterAction(BaseAction* a) {
		if (!default_actions_) {
			default_actions_ = new ActionListType();
		}
		default_actions_->push_back(a);
	}

	void FrameWork::ResponseError(HttpStatusCode code, const char* message, Response* response) {
		// TODO: 貌似返回非 200 有错误, nginx 不识别？
		GenerateCommonHeader(&response->header(), code, message);
		response->OutputHeader();

		// TODO: status code to template file name
		/*#if 0
		  ctemplate::TemplateDictionary error_dict_("error"); // TODO: use member?
		  ctemplate::Template* tpl = ctemplate::Template::GetTemplate(
		  "404.tpl", ctemplate::STRIP_WHITESPACE);

		  ASSERT(tpl);
		  if (tpl)
		  tpl->Expand(response, &error_dict_);
#endif*/
	}

}
