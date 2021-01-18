#include "libs.h"

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

void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void updateInput(GLFWwindow* window, Mesh &mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.scaleUp(glm::vec3(-0.1f));
	}
}

int main()
{
	// INIT GLFW
	glfwInit();

	// CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
	
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	//
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window); // IMPORTANT!!

	// INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	// Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	// OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SHADER INIT
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");

	// MODEL MESH
	Mesh test(vertices, nrOfVertices, indices, nrOfIndices,
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);
	
	// TEXTURE INIT

	// TEXTURE 0
	Texture texture0("Images/Texture.png", GL_TEXTURE_2D, 0);

	// TEXTURE 1
	Texture texture1("Images/Texture1.png", GL_TEXTURE_2D, 1);

	// MATERIAL 0
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	// INIT MATRICES
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);

	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth / framebufferHeight) ,
		nearPlane,
		farPlane
	);

	// LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 2.f);

	// INIT UNIFORMS
	core_program.use();

	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");

	glUseProgram(0);

	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		// UPDATE INPUT ---
		glfwPollEvents();
		updateInput(window, test);
		// UPDATE ---
		updateInput(window);

		// DRAW ---
		// Clear
		glClearColor(0.2f, 0.3f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Update uniforms
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");
		material0.sendToShader(core_program);

		// Update framebuffer size and projection matrix
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth / framebufferHeight),
			nearPlane,
			farPlane
		);

		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		// Use a program
		core_program.use();

		// Activate texture
		texture0.bind();
		texture1.bind();

		// Draw
		test.render(&core_program);

		// End Draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	// DELETE VAO and Buffers

	return 0;
}