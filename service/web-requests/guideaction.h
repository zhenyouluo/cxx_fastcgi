/*==================================
* Copyright (C) 2014 All rights reserved.
* 
* file：guideaction.h
* author：122801281@qq.com
* date：2014-11-28
* description：
*
================================================================*/
#ifndef CWF_GUIDEACTION_H__
#define CWF_GUIDEACTION_H__

#include "cwf/action.h"

namespace cwf {

	class GuideAction : public BaseAction {
		public:
			GuideAction();
			virtual bool Match(const std::string& url) const;
			virtual HttpStatusCode Process(Request*, Response*);
		private:
			void SetHeader(Response*);
		private:
			static int counter_;
	};

}

#endif

