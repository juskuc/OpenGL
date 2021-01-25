#include "Game.h"

// General functions

// Private functions
void Game::initGLFW()
{
	// INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char* title,
	bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR:GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);
	//
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); // IMPORTANT!!
}

void Game::initGLEW()
{
	// INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	// Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth / this->framebufferHeight),
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	// TEXTURE 0
	this->textures.push_back(new Texture("Images/Texture.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/Texture_specular.png", GL_TEXTURE_2D));

	// TEXTURE 1
	this->textures.push_back(new Texture("Images/Texture1.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/Texture1_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Game::initMeshes()
{
	Pyramid tempQuad = Pyramid();
	this->meshes.push_back(new Mesh(&tempQuad,
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
		)
	);

	this->meshes.push_back(new Mesh(&tempQuad,
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	)
	);
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
	// INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
}

void Game::updateUniforms()
{
	// Update view matrix (camera)
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	// Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	//this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth / this->framebufferHeight),
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

// Constructors/Destructors
Game::Game(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR)
{
	// Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.0f;
	this->currTime = 0.f;
	this->lastTime = 0.f;
	
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];
	
	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (size_t i = 0; i < this->meshes.size(); i++)
		delete this->meshes[i];

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
}

// Accessors
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

// Modifiers
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Functions
void Game::updateDt()
{
	this->currTime = static_cast<float>(glfwGetTime());
	this->dt = this->currTime - this->lastTime;
	this->lastTime = this->currTime;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	// Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	// Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::updateKeyboardInput()
{
	// Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwWindowShouldClose(this->window);
	}

	// Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camPosition.z -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camPosition.z += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camPosition.x -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camPosition.x += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.05f;
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
}

void Game::update()
{
	// Update input
	this->updateDt();
	this->updateInput();

	std::cout << "DT: " << this->dt << "\n"
		<< "Mouse offsetX: " << this->mouseOffsetX << " Mouse offsetY: " << this->mouseOffsetY << "\n";
}

void Game::render()
{
	// DRAW ---
	// Clear
	glClearColor(0.2f, 0.3f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Update the uniforms
	this->updateUniforms();

	// Update uniforms
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	// Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();

	// Activate texture
	this->textures[TEX_1]->bind(0);
	this->textures[TEX_1_SPECULAR]->bind(1);

	// Draw
	this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);

	// End Draw
	glfwSwapBuffers(this->window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

// Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}
