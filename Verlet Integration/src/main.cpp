#include "Core/Window.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include "File/File.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/Camera/Camera.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Material/Texture/TextureConstructor.h"
#include "Rendering/Material/Material.h"
#include "Rendering/Debug/DebugDraw.h"
#include "World/Model/Model.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtx/euler_angles.hpp>

// TODO:
// Add models and then mesh loading
// Setup verlet stuff

int main()
{
	Verlet::Log::Init();

	Verlet::Window* window = new Verlet::Window();
	GLFWwindow* glfwWindow = window->GetGLFWWindow();

	Verlet::Camera* camera = new Verlet::Camera(glm::vec3(0.0f));
	Verlet::Shader* shader = new Verlet::Shader("res:/shaders/main.vert", "res:/shaders/main.frag");
	Verlet::Model* model = new Verlet::Model("res:/models/floor.obj", shader, true);
	Verlet::Model* model2 = new Verlet::Model("res:/models/box.obj", shader, true);

	Verlet::grDebugDraw::Init();

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(glfwWindow))
	{
		double now = glfwGetTime();
		double deltaTime = now - lastTime;
		lastTime = now;

		Verlet::Time::Time = now;
		Verlet::Time::DeltaTime = deltaTime;
		Verlet::Time::Frame++;

		// Input processing
		window->InputStep();
		camera->ProcessKeyboard();
		camera->ProcessMouse();

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Verlet::grDebugDraw::Update();
		Verlet::grDebugDraw::Line(glm::vec3(0.0f), glm::vec3(5.0f) * (float)sin(now * 0.5), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Verlet::grDebugDraw::Sphere(glm::mat4(1.0f) * glm::eulerAngleXYZ((float)glm::radians(now * 15), (float)-glm::radians(now * 15), 0.0f), 1, 20);
		Verlet::grDebugDraw::Box(glm::mat4(1.0f), glm::vec3(1.0f) * 1.25f, glm::vec3(0, 1, 0));
		Verlet::grDebugDraw::Draw(camera);

		model->Draw(glm::mat4(1.0f), camera);
		model2->Draw(glm::translate(glm::mat4(1.0f), glm::vec3(3, 2.19 / 2, 1.5)), camera);

		// Swap buffers, Events
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

