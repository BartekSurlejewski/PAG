#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/intersect.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#define SHADOW_WIDTH  1024
#define SHADOW_HEIGHT  1024


Core* core = nullptr;

float xOffset = 0.0f;
float yOffset = 0.0f;

bool directionalLightOn = false;
bool pointLightOn = false;
bool spotLightOn = false;

int UnProject(float winX, float winY, float winZ, const glm::mat4& matViewProjection, const int viewport[4], float* objX, float* objY, float* objZ)
{
	glm::vec4 in;
	in.x = (winX - viewport[0]) * 2 / viewport[2] - 1.0f;
	in.y = (winY - viewport[1]) * 2 / viewport[3] - 1.0f;
	in.z = winZ;
	in.w = 1.0f;

	glm::vec4 out = in * matViewProjection;

	if (out.w == 0.0f)
	{
		return 0;
	}
	*objX = out.x / out.w;
	*objY = out.y / out.w;
	*objZ = out.z / out.w;

	return 1;
}

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

bool Intersect(const Ray& r, const glm::vec3& min, const glm::vec3& max)
{
	float tmin = (min.x - r.origin.x) / r.direction.x;
	float tmax = (max.x - r.origin.x) / r.direction.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (min.y - r.origin.y) / r.direction.y;
	float tymax = (max.y - r.origin.y) / r.direction.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - r.origin.z) / r.direction.z;
	float tzmax = (max.z - r.origin.z) / r.direction.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

int UnprojectFromScreen(float x1, float y1, float z1, float* x2, float* y2, float* z2, const glm::mat4& world)
{
	float modelMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	float invViewProjectionMatrix[16];

	int viewport[4];
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = 1280;
	viewport[3] = 720;

	glm::mat4 matView = core->GetCamera()->view;
	glm::mat4 matProj = core->GetCamera()->projection;
	glm::mat4 matWorld = glm::transpose(world);
	matView = glm::transpose(matView);
	matProj = glm::transpose(matProj);
	glm::mat4 matViewProj = matWorld * matView * matProj;
	glm::mat4 invMatViewProj = glm::inverse(matViewProj);

	memcpy(invViewProjectionMatrix, &invMatViewProj, 16 * sizeof(float));

	return UnProject(x1, y1, z1, invMatViewProj, viewport, x2, y2, z2);
}

glm::vec3 UnprojectFromScreen(int x, int y, const glm::mat4& world)
{
	glm::vec3 dir, center;

	UnprojectFromScreen((float)x, (float)y, 0.f, &center.x, &center.y, &center.z, world);
	UnprojectFromScreen((float)x, (float)y, 1.f, &dir.x, &dir.y, &dir.z, world);

	char chBuffer[256];
	sprintf_s(chBuffer, "Vec0: %f, %f, %f\nVec1:%f, %f, %f\nCameraPos: %f, %f, %f\n\n", center.x, center.y, center.z, dir.x, dir.y, dir.z, core->GetCamera()->GetPosition().x,
		core->GetCamera()->GetPosition().y, core->GetCamera()->GetPosition().z
	);

	return glm::normalize(dir - center);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	TwWindowSize(width, height);
}

void make_list(SceneNode* node, std::vector<SceneNode*>& nodesList)
{
	for (int i = 0; i < node->children.size(); ++i)
	{
		SceneNode* child = node->children[i];
		nodesList.push_back(child);

		make_list(child, nodesList);
	}
}

void mouse_button(GLFWwindow* window, int button, int x, int y)
{
	TwEventMouseButtonGLFW(button, x);

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		core->canMoveCamera = true;
	}
	else
	{
		core->canMoveCamera = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && x == 0)
	{
		glm::vec3 cameraPos = core->GetCamera()->GetPosition();

		Ray ray;
		ray.origin = cameraPos;

		// intersect test
		std::vector<SceneNode*> sceneNodes;
		make_list(core->model.graph.rootNode, sceneNodes);

		bool bIntersect = false;

		TwRemoveAllVars(core->bar);

		for (int i = 0; i < sceneNodes.size(); ++i)
		{
			if (sceneNodes[i]->nodeType != Type_Model)
			{
				continue;
			}
			ModelNode* modelNode = (ModelNode*)(sceneNodes[i]);

			const glm::mat4& world = modelNode->combinedXForm;

			glm::vec4 boundaryMin = glm::vec4(modelNode->boundingBoxMin, 1.0f);
			glm::vec4 boundaryMax = glm::vec4(modelNode->boundingBoxMax, 1.0f);
			boundaryMin = world * boundaryMin;
			boundaryMax = world * boundaryMax;

			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);

			glm::vec3 direction = UnprojectFromScreen((int)xPos, (int)yPos, world);
			ray.direction = direction;

			// Sphere
			glm::vec4 sphereCenter = glm::vec4(modelNode->boundingSphereCenter, 1.0f);

			glm::mat4 matScaleUnitSphere;
			matScaleUnitSphere = glm::scale(matScaleUnitSphere, glm::vec3(modelNode->boundingSphereRadius));

			sphereCenter = world * sphereCenter;
			float sphereCenterSqared = powf(modelNode->boundingSphereRadius, 2.5f);

			glm::vec3 sphereCenterXYZ = glm::vec3(sphereCenter.x, sphereCenter.y, sphereCenter.z);

			float dist = 0;
			//bIntersect = glm::intersectRaySphere(ray.origin, ray.direction, sphereCenterXYZ, sphereCenterSqared, dist);

			if (bIntersect == true)
			{
				char ch[32];
				sprintf_s(ch, "intersect %d\n", i);
				OutputDebugStringA(ch);

				ModelNode* pIntersectedNode = modelNode;
				pIntersectedNode->color = glm::vec4(5.0f, 0.0f, 0.0f, 1.0f);

				char buf[64];
				sprintf_s(buf, "Group=%s step=0.1", pIntersectedNode->name.c_str());

				TwAddVarRW(core->bar, std::string("translate x").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.translate.x, buf);
				TwAddVarRW(core->bar, std::string("translate y").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.translate.y, buf);
				TwAddVarRW(core->bar, std::string("translate z").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.translate.z, buf);

				TwAddVarRW(core->bar, std::string("scale x").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.scale.x, buf);
				TwAddVarRW(core->bar, std::string("scale y").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.scale.y, buf);
				TwAddVarRW(core->bar, std::string("scale z").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.scale.z, buf);

				TwAddVarRW(core->bar, std::string("rotate x").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.rotate.x, buf);
				TwAddVarRW(core->bar, std::string("rotate y").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.rotate.y, buf);
				TwAddVarRW(core->bar, std::string("rotate z").c_str(), TW_TYPE_FLOAT, &pIntersectedNode->xForm.rotate.z, buf);

			}
			else
			{
				modelNode->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			}
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 1280 / 2;
	static float lastY = 720 / 2;


	xOffset = xpos - lastX;
	yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.5;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	if (core->canMoveCamera)
	{
		Camera* camera = core->GetCamera();

		camera->SetYaw(camera->GetYaw() + xOffset);
		camera->SetPitch(camera->GetPitch() + yOffset);
		camera->UpdateOrientation();
	}

	TwEventMousePosGLFW((int)xpos, (int)ypos);
}

Core::Core()
{

}

bool Core::Initialize()
{
	if (!glfwInit())
	{
		return false;
	}

	core = this;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window.Initialization(1280, 720);

	camera.SetAspectRatio(1280.f / 720.f);
	camera.SetPosition(glm::vec3(0.0f, 2.0f, 30.0f));

	glfwMakeContextCurrent(window.GetWindow());
	glfwSetFramebufferSizeCallback(window.GetWindow(), framebuffer_size_callback);

	//glfwSetInputMode( m_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glfwSetCursorPosCallback(window.GetWindow(), mouse_callback);

	glfwSetMouseButtonCallback(window.GetWindow(), mouse_button);


	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}


	// Init AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);

	bar = TwNewBar("Hierarchy");
	TwWindowSize(1280, 720);
	TwDefine(" Hierarchy position='1060 20' ");

	SetLights();

	barLighting = TwNewBar("Lighting");
	TwAddVarRW(barLighting, "Direction", TW_TYPE_DIR3F, &directionalLight->GetDirection(), "Group=Directional");

	TwAddVarRW(barLighting, "Const", TW_TYPE_FLOAT, &pointLight->GetAttenuation().x, "Group=Point");
	TwAddVarRW(barLighting, "Linear", TW_TYPE_FLOAT, &pointLight->GetAttenuation().y, "Group=Point step=0.001 min=0");
	TwAddVarRW(barLighting, "Quadratic", TW_TYPE_FLOAT, &pointLight->GetAttenuation().z, "Group=Point step=0.0001 min=0");

	TwAddVarRW(barLighting, "Direction", TW_TYPE_DIR3F, &spotLight->GetDirection(), "Group=Spot");
	TwAddVarRW(barLighting, "Position", TW_TYPE_DIR3F, &spotLight->GetPosition(), "Group=Spot");

	// set inital viewport
	glViewport(0, 0, 1280, 720);

	//Depth map FBO configuration
	glGenFramebuffers(1, &depthMapFBO);

	//Create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// INITIALIZE MESH DATA
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	std::vector<Vertex> verts;
	for (int i = 0; i < 36; ++i)
	{
		int offset = i * 5;

		Vertex v;
		v.Position = glm::vec3(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
		v.TexCoords = glm::vec2(vertices[offset + 3], vertices[offset + 4]);
		verts.push_back(v);
	}

	glEnable(GL_DEPTH_TEST);

	std::vector<unsigned int> indices;

	model.LoadModel("Models/Hierarchia.3ds");
	model2.LoadModel("Models/Nanosuit/nanosuit.obj");
	plane.LoadModel("Models/plane/plane.obj");

	shader = Shader("Shaders/shader.vert", "Shaders/shader.frag");

	return true;
}

void Core::Update()
{
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		float currentTime = glfwGetTime();
		float delta = currentTime - lastTime;
		lastTime = currentTime;

		processInputCore(window.GetWindow(), delta);

		Render();

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Core::processInputCore(GLFWwindow* pWindow, float delta)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
	if (glfwGetKey(pWindow, GLFW_KEY_T) == GLFW_PRESS)
	{
		directionalLightOn = !directionalLightOn;
	}

	camera.processInput(pWindow, delta);
}

void Core::Render()
{
	glClearColor(0.8f, 0.7f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.UseProgram();

	shader.SetMat4("view", camera.view);
	shader.SetMat4("projection", camera.projection);
	shader.SetVec3("cameraPosition", camera.GetPosition());

	////Render to depth map
	//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	DrawScene();

	return;
}

void Core::DrawScene()
{
	/******SCENE RENDERRING******/

	Material modelMaterial;
	modelMaterial.specFactor = 10;
	modelMaterial.SetSimple(shader);

	Transform lightTransform;
	directionalLight->Draw(lightTransform, &shader);

	glm::vec3 pos;
	pos.y = 0.0f;
	pos.x = 20 * sinf(glfwGetTime() * 0.5);
	pos.z = 20 * cosf(glfwGetTime() * 0.5);
	pointLight->SetPosition(pos);
	pointLight->Draw(lightTransform, &shader);

	spotLight->Draw(lightTransform, &shader);

	for (int x = 0; x <= 2; x++)
	{
		for (int z = 1; z <= 3; z++)
		{
			Transform trans;
			trans.translate = glm::vec3((float)x * 10, 0.0f, (float)z * -10);
			trans.CalculateWorldMatrix();

			shader.SetMat4("model", trans.worldMatrix);
			model2.Draw(shader);
		}
	}

	modelMaterial.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	modelMaterial.specFactor = 64;
	modelMaterial.SetSimple(shader);
	model.DrawAsGraph(shader);

	modelMaterial.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	modelMaterial.specFactor = 20;
	modelMaterial.SetSimple(shader);

	Transform trans;
	trans.translate = glm::vec3(0.0f, -10.0f, 0.0f);
	trans.CalculateWorldMatrix();

	shader.SetMat4("model", trans.worldMatrix);
	plane.Draw(shader);

	TwDraw();
}

void Core::SetLights()
{
	directionalLight = new DirectionalLight();

	Material matDirectional;
	matDirectional.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	matDirectional.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	directionalLight->SetMaterial(matDirectional);

	// point light
	pointLight = new PointLight();

	Material matPoint;
	matPoint.ambient = matPoint.specular = matPoint.diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
	pointLight->SetMaterial(matPoint);


	// spot light
	spotLight = new SpotLight();

	Material matSpot;
	matSpot.ambient = matSpot.specular = matSpot.diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
	spotLight->SetMaterial(matSpot);
}
