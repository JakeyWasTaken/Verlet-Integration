#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include "World/Model/Model.h"
#include "World/Object/Object.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "Engine/Engine.h"

#define Color(r, g, b) glm::vec3((float)r##/255.0f, (float)g##/255.0f, (float)b##/255.0f)

// TODO:
// Setup verlet stuff

using namespace Verlet;

int main()
{
	Engine* engine = new Engine();
	engine->Init();

	Shader* shader = new Shader("res:/shaders/main.vert", "res:/shaders/main.frag");
	Model* floorModel = new Model("res:/models/floor.obj", shader, true);
	Model* boxModel = new Model("res:/models/box.obj", shader, true);
	Model* flagModel = new Model("res:/models/flag.obj", shader, true, MESH_FLAG_DOUBLE_SIDED);

	Object* floor = new Object(floorModel);
	Object* box = new Object(boxModel);
	Object* flag = new Object(flagModel);
	flag->position = glm::vec3(0, 5, 0);

	engine->GetScene()->PushObject(floor);
	//engine->GetScene()->PushObject(box);
	engine->GetScene()->PushObject(flag);

	engine->Run();
	
	return 0;
}

