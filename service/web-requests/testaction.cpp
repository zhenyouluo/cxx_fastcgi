#include "testaction.h"
#include "cwf/stream.h"

namespace cwf {
	int TestAction::counter_ = 0;
	TestAction::TestAction(){
	}
	bool TestAction::Match(const std::string& url) const {
		return url.compare("/testaction") == 0;
	}
	HttpStatusCode TestAction::Process(Request* req, Response* res) {
		std::ostringstream oss;
		oss << "TestAction::Process() counter:" << (++counter_);
		SetHeader(res);
		res->WriteRaw("testaction");
		return cwf::HC_OK;
	}

	void TestAction::SetHeader(Response* res) {
		static const std::string kDefaultContentType("text/html; charset=utf-8");
		res->header().set_status_code(cwf::HC_OK, "OK");
		res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
		res->OutputHeader();
	}

}

