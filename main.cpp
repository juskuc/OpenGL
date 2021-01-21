#include "libs.h"
#include "Game.h"

Primitive test();

Vertex vertices[] =
{
	// Position							// Color						// Text						// Normals
	glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,	// Triangle 1
	0, 2, 3 	// Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

int main()
{
	Game game(
		"OpenGL",
		640, 480,
		4, 4,
		false);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		// UPDATE INPUT ---
		game.update();
		game.render();
	}

	return 0;
}