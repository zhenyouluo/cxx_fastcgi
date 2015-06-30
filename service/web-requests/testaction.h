#ifndef CWF_TESTACTION_H__
#define CWF_TESTACTION_H__

#include "cwf/action.h"

namespace cwf {

	class TestAction : public BaseAction {
		public:
			TestAction();
			virtual bool Match(const std::string& url) const;
			virtual HttpStatusCode Process(Request*, Response*);
		private:
			void SetHeader(Response*);
		private:
			static int counter_;
	};

}

#endif

