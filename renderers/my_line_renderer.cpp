#include "my_line_renderer.h"
#include <helpers.h>
#include <graphic_api.h>

MyLineRenderer::MyLineRenderer() : Renderer("", Shader(sl::shadersPath("gizmo_line.vert"), sl::shadersPath("gizmo_line.frag"))) {
		
}

void MyLineRenderer::init() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shader.use();

	// VBO
	sl::createVBOVec3(shader.id, "p", sizeof(LineComponent), (void*)offsetof(LineComponent, p));
	sl::createVBOVec3(shader.id, "color", sizeof(LineComponent), (void*)offsetof(LineComponent, color));

	std::cout << "MyLineRenderer init" << std::endl;
}

void MyLineRenderer::push() {

	std::vector<LineComponent> lineComponents;

	// Map Line -> LineComponent for VBO
	for (auto &l : lines) {
		lineComponents.push_back({.p = l.a, .color = l.color});
		lineComponents.push_back({ .p = l.b, .color = l.color });
	}

	writeVBOBuffer(lineComponents, true);
}

void MyLineRenderer::render(glm::vec3 &position) {

	if (!visible)
		return;

	glBindVertexArray(VAO);

	setPosition(position);

	glDrawArrays(GL_LINES, 0, nelements);
}

void MyLineRenderer::clear() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nelements * sizeof(LineComponent), nullptr, GL_DYNAMIC_DRAW);
	clearLines();
}

void MyLineRenderer::clean() {
	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.clean();
}