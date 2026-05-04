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
#include <data/element_type.h>

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
	}

	// TODO create a register function that need to be override

	void hello() {
		std::cout << "hello" << std::endl;
	}

};