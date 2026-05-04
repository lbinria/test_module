function init()

	-- Call a C++ function from script.h
	test_module.hello()

	if app.is_debug then 
		print("load model for debug...")
		if not app.scene:load_model("assets/catorus_tri.geogram", "cat") then 
			print("fail to load model.")
		end
		print("model loaded !")
	end

end

function draw_gui()

	-- Create a new window
	imgui.Begin("test_module")

	-- Check if model some_model exists
	if app.scene.models:has("some_model") then 
		if imgui.Button("Save created mesh##test_model.btn_save") then 
			app:save_model("some_model", "some_model.geogram") -- Replace this function doesnt exists anymore
		end
	end

	imgui.End()

end

local lastIdx = {}

function get_vertex_attr(model)

	-- Don't know the type of model => more generic way to handle attribute binding
	-- local attr = model:bind_attr("_highlight", ElementKind.POINTS_ELT, ElementType.BOOL_ELT)
	-- attr.bool_data[1] = true

	local pa = PointAttributeBool.new()
	local poly_model = model:as_poly()
	pa:bind("_highlight", poly_model.surface_attributes, poly_model.mesh)
	return pa
end

function create_point()
	local st = app.input_state

	-- Add or get model
	local model = app.scene.models:add("PolyModel", "some_model")
	-- Setup gfx
	if not model.points.visible then 
		model.points.visible = true
		model.points.size = 25.
		model.points.light = true
		model.mesh.size = 1.
	end

	local sel_vi = -1
	if st.vertex.any_hovered then 
		sel_vi = st.vertex.hovered
		-- highlight
		local pa = get_vertex_attr(model)
		pa[sel_vi] = true
		model:set_highlight(ElementKind.POINTS_ELT)
	else 	
		local mouse_pos = st.mouse.pos
		local p = app:pick_point(mouse_pos.x, mouse_pos.y)
		-- print("click point: " .. p:to_string())
		sel_vi = model.points:add_point(p)
	end 

	lastIdx[#lastIdx+1] = sel_vi
	if #lastIdx == 4 then 
		local f = model:create_elements(1, 4)
		for lv, vi in ipairs(lastIdx) do 
			-- print("f: " .. tostring(f) .. ", lv: " .. tostring(lv - 1) .. ", vi:" .. tostring(vi))
			model:vert(f, lv - 1, vi)
		end
		lastIdx = {}

		local pa = get_vertex_attr(model) 
		pa:fill(false)
		model:unset_highlight(ElementKind.POINTS_ELT)
	end

	model:push()


end

function delete_facet()
	local f = app.input_state.facet.hovered
	print("facet to delete: " .. tostring(f))
	-- Add or get model
	local model = app.scene.models:add("PolyModel", "some_model")
	model:delete_elements({f})
	model:delete_isolated_vertices()

	model:push()
end

function mouse_button(button, action, mods)


	local st = app.input_state

	if not st.mesh.any_hovered then 
		return
	end

	-- Check mesh
	
	print("go")
	-- local hovered_model = app.scene.models:missing_function()
	local hovered_model = app.scene.models:get_by_index(st.mesh.hovered)
	print(hovered_model)

	if hovered_model then 
		print("click")
		print("clock on: " .. tostring(hovered_model.name))
	else 
		print("nullos")
	end

	if button == 0 and action == 1 and mods == 2 then 
		create_point()
	elseif button == 1 and action == 1 and mods == 2 and hovered_model.name == "some_model" and st.facet.any_hovered then
		delete_facet()
	end




end


function update(dt)

end