#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

// Salamesh core
#include <app_interface.h>
#include <script.h>
#include <input_states.h>
#include <models/model.h>
#include <models/volume_model.h>
#include <models/surface_model.h>
#include <renderers/line_renderer.h>
#include <data/element_type.h>

#include "renderers/my_line_renderer.h"

// Ultimaille
#include <ultimaille/all.h>
// ImGui
// #include <imgui.h>
// #include "imgui.h"
// #include "imgui_internal.h"

// Std
#include <iostream>

struct MyScript final : public Script {

	// Modify constructor body in order to
	// register your functions / types here... using sol2 lib
	MyScript(IApp &app, sol::state &lua) : Script(app) {
		
		auto test_module = lua.create_table();
		lua["test_module"] = test_module;

		// Example, registering some functions to lua script
		test_module.set_function("hello", &MyScript::hello, this);

		// test_module.set_function("test", sol::overload(
		// 	[](MyScript &self, SurfaceAttributes &attr) {
		// 		return self.test(attr);
		// 	},
		// 	[](MyScript &self, sol::object o) {
		// 		return self.test(o);
		// 	}

		// 	// sol::resolve<void(SurfaceAttributes&)>(&MyScript::test),
		// 	// sol::resolve<void(sol::object)>(&MyScript::test)
		// ));

		test_module.set_function("test1", sol::resolve<void(SurfaceAttributes&)>(&MyScript::test), this);
		test_module.set_function("test2", sol::resolve<void(sol::object)>(&MyScript::test), this);
		test_module.set_function("display_lines", &MyScript::displayLines, this);




		// auto t = RendererBindings::add<MyLineRenderer>("MyLineRenderer"); --> that do the following

		sol::table renderer_metatable = lua["Renderer"];
		renderer_metatable.set_function("as_my_line_renderer", [](Renderer &self) {
			return dynamic_cast<MyLineRenderer*>(&self);
		});
		// renderer_metatable.set_function("as_my_line_renderer", [](std::shared_ptr<Renderer> &self) {
		// 	return std::static_pointer_cast<MyLineRenderer>(self);
		// });

		sol::usertype<MyLineRenderer> myLineRenderer_t = lua.new_usertype<MyLineRenderer>(
			"MyLineRenderer",
			sol::base_classes, 
			sol::bases<Renderer>(),
			"clear_lines", &MyLineRenderer::clearLines,
			"add_line", &MyLineRenderer::addLine,
			"push", &MyLineRenderer::push,
			"auto_update", sol::writeonly_property(&MyLineRenderer::setAutoUpdate)
		);

		auto line_t = lua.new_usertype<MyLineRenderer::Line>(
			"MyLine",
			sol::call_constructor, [](sol::table t) {
				MyLineRenderer::Line line;
				
				if (t["a"].valid() && t["a"].is<glm::vec3>())
					line.a = t["a"].get<glm::vec3>();
				if (t["b"].valid() && t["b"].is<glm::vec3>())
					line.b = t["b"].get<glm::vec3>();
				if (t["color"].valid() && t["color"].is<glm::vec3>())
					line.color = t["color"].get<glm::vec3>();
				
				return line;
			},
			"a", &MyLineRenderer::Line::a,
			"b", &MyLineRenderer::Line::b,
			"color", &MyLineRenderer::Line::color
		);
	}

	// TODO create a register function that need to be override

	void hello() {
		std::cout << "hello" << std::endl;
	}

	void test( SurfaceAttributes& attrs) {
		std::cout << "attrs point size: " << attrs.points.size() << std::endl;
	}

	void test(sol::object obj) {
		if (obj.is<SurfaceAttributes&>()) {
			std::cout << "attrs point size: " << obj.as<SurfaceAttributes&>().points.size() << std::endl;
		} else {
			std::cout << "argument fail" << std::endl;
		}
	}

	void displayLines() {
		auto r = app.getScene().getRenderers().add("MyLineRenderer", "boob");
		auto lr = std::static_pointer_cast<MyLineRenderer>(r);
		lr->clearLines();
		lr->addLine(MyLineRenderer::Line{{0,0,0}, {1,0,0}, {1,0,0}});
		lr->push();
	}

};