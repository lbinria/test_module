// NOT MODIFY HERE !
// interface is used by the viewer to initialize the class
// class will register all custom functions & types needed into lua
// Note: You should modify register constructor implementation to register your own functions & types
#include "script.h"
#include "renderers/renderer_info.h"
#include "renderers/my_line_renderer.h"

static int rInfosSize = 0;

#if defined(__linux__) || defined(__APPLE__)

extern "C" {
	MyScript* allocator(IApp &app, sol::state &lua) {
		return new MyScript(app, lua);
	}

	void deleter(MyScript *ptr) {
		delete ptr;
	}

	RendererInfo** allocateRendererInfos() {
		rInfosSize = 1;
		RendererInfo ** rInfosPtr = new RendererInfo*[rInfosSize];
		rInfosPtr[0] = new RendererInfo{
			"MyLineRenderer",
			[](auto _) { return std::make_unique<MyLineRenderer>(); }
		};

		return rInfosPtr;

		// std::vector<RendererInfo*> rInfos{
		// 	new RendererInfo{ "MyLineRenderer", []() { return std::make_unique<MyLineRenderer>(); } }
		// };
		
		// rInfosSize = rInfos.size();
		// RendererInfo** rInfosPtr = rInfos.data();

		// return std::move(rInfosPtr);
	}

	int rendererInfosSize() {
		return rInfosSize;
	}

}

#endif

#ifdef WIN32
extern "C"
{
	__declspec (dllexport) MyScript* allocator(IApp &app, sol::state &lua) {
		return new MyScript(app, lua);
	}

	__declspec (dllexport) void deleter(MyScript *ptr) {
		delete ptr;
	}

}
#endif