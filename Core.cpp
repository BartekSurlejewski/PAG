#include "stdafx.h"
#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/intersect.hpp>
#include <AntTweakBar/AntTweakBar.h>
#include <string>
#include <iostream>

Core* pCore = nullptr;

Camera* globalCamera = nullptr;
float xOffset = 0.0f;
float yOffset = 0.0f;
bool isCursorEnabled;

int sUnProject(float winx, float winy, float winz, const glm::mat4& matInvViewProj, const int viewport[4], float* objx, float* objy, float* objz)
{
	glm::vec4 vIn;
	vIn.x = (winx - viewport[0]) * 2 / viewport[2] - 1.0f;
	vIn.y = (winy - viewport[1]) * 2 / viewport[3] - 1.0f;
	vIn.z = winz;
	vIn.w = 1.f;

	glm::vec4 vOut = vIn*matInvViewProj;

	if (vOut.w == 0.0f)
	{
		return 0;
	}

	*objx = vOut.x / vOut.w;
	*objy = vOut.y / vOut.w;
	*objz = vOut.z / vOut.w;

	return 1;
}

struct SRay
{
	glm::vec3 orig;
	glm::vec3 dir;
};

//------------------------------------------------------------------------
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
//------------------------------------------------------------------------
bool Intersect(const SRay& r, const glm::vec3& min, const glm::vec3& max)
{
	float tmin = (min.x - r.orig.x) / r.dir.x;
	float tmax = (max.x - r.orig.x) / r.dir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (min.y - r.orig.y) / r.dir.y;
	float tymax = (max.y - r.orig.y) / r.dir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - r.orig.z) / r.dir.z;
	float tzmax = (max.z - r.orig.z) / r.dir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

int UnprojectFromScreen(float sX, float sY, float sZ, float* pX, float* pY, float* pZ, const glm::mat4& world)
{
	float modelMatrix[16];
	float viewMatrix[16];
	float projMatrix[16];
	float invViewProjMatrix[16];

	int viewport[4];
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = 1280;
	viewport[3] = 720;

	glm::mat4 matView = pCore->camera->view;
	glm::mat4 matProj = pCore->camera->projection;


	glm::mat4 matWorld = glm::transpose(world);
	matView = glm::transpose(matView);
	matProj = glm::transpose(matProj);

	glm::mat4 matViewProj = matView * matProj;
	glm::mat4 matInvViewProj = glm::inverse(matViewProj);

	memcpy(invViewProjMatrix, &matInvViewProj, 16 * sizeof(float));

	return sUnProject(sX, sY, sZ, matInvViewProj, viewport, pX, pY, pZ);
}


//------------------------------------------------------------------------
glm::vec3 UnprojectFromScreen(int x, int y, const glm::mat4& world)
{
	glm::vec3 vDir, vCenter;

	UnprojectFromScreen((float)x, (float)y, 0.f, &vCenter.x, &vCenter.y, &vCenter.z, world);
	UnprojectFromScreen((float)x, (float)y, 1.f, &vDir.x, &vDir.y, &vDir.z, world);

	char chBuffer[256];
	sprintf_s(chBuffer, "Vec0: %f, %f, %f\nVec1:%f, %f, %f\nCameraPos: %f, %f, %f\n\n", vCenter.x, vCenter.y, vCenter.z, vDir.x, vDir.y, vDir.z, pCore->camera->cameraPos.x,
		pCore->camera->cameraPos.y, pCore->camera->cameraPos.z
	);
	//OutputDebugStringA(chBuffer);


	return glm::normalize(vDir - vCenter);

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!TwEventMousePosGLFW((int)xpos, (int)ypos))
	{
		static float lastX = 1000 / 2;
		static float lastY = 640 / 2;

		xOffset = xpos - lastX;
		yOffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.05;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		globalCamera->yaw += xOffset;
		globalCamera->pitch += yOffset;
		globalCamera->updateVectors();
	}
}

void make_list(GraphNode* pNode, std::vector<GraphNode*>& nodesList)
{
	for (unsigned int i = 0; i < pNode->children.size(); ++i)
	{
		GraphNode* pChild = pNode->children[i];
		nodesList.push_back(pChild);

		make_list(pChild, nodesList);
	}
}

void mouse_button(GLFWwindow* window, int button, int x, int y)
{
	TwEventMouseButtonGLFW(button, x);

	if (button == GLFW_MOUSE_BUTTON_RIGHT && x==0)
	{
		if(isCursorEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		isCursorEnabled = !isCursorEnabled;
	}
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && x == 1)
	{
		glm::vec3 vCameraPos = pCore->camera->cameraPos;

		SRay ray;
		ray.orig = vCameraPos;

		// intersect test
		std::vector<GraphNode*> sceneNodes;
		make_list(pCore->scene->rootNode, sceneNodes);

		bool bIntersect = false;

		// remove all vars from anttweakbar's window
		TwRemoveAllVars(pCore->barHierarchy);

		for (unsigned int i = 0; i < sceneNodes.size(); ++i)
		{
			// Process only Models
			//if (sceneNodes[i]->GetNodeType() != ENodeType_Model)
			//{
			//	continue;
			//}


			GraphNode* pModelNode = sceneNodes[i];

			if (pModelNode->meshes.size() > 0)
			{


				glm::vec3 vAABBMin, vAABBMax;
				pModelNode->GetAABB(vAABBMin, vAABBMax);

				//const glm::mat4& world = sceneNodes[i]->combinedXform;
				const glm::mat4& world = glm::mat4(1.0f);


				glm::vec4 aabbMin = glm::vec4(vAABBMin, 1.0f);
				glm::vec4 aabbMax = glm::vec4(vAABBMax, 1.0f);
				aabbMin = world * aabbMin;
				aabbMax = world * aabbMax;

				double xPos, yPos;
				glfwGetCursorPos(window, &xPos, &yPos);


				glm::vec3 vDirection = UnprojectFromScreen((int)xPos, (int)yPos, world);
				ray.dir = vDirection;

				// Sphere
				glm::vec4 sphereCenter = glm::vec4(pModelNode->boundingSphereCenter, 1.0f);
				sphereCenter = world * sphereCenter;

				char chBuffer[256];
				sprintf_s(chBuffer, "%f %f %f\n", sphereCenter.x, sphereCenter.y, sphereCenter.z);
				OutputDebugStringA(chBuffer);

				float sphereCenterSqared = powf(pModelNode->boundingSphereRadius * sceneNodes[i]->local.scale.x, 2.f);

				glm::vec3 sphereCenterXYZ = glm::vec3(sphereCenter.x, sphereCenter.y, sphereCenter.z);

				float dist = 0;
				//bIntersect = glm::intersectRaySphere(ray.orig, ray.dir, sphereCenterXYZ, sphereCenterSqared, dist);

				bIntersect = Intersect(ray, aabbMin, aabbMax);

				if (bIntersect == true)
				{
					char ch[32];
					sprintf_s(ch, "intersect %d\n", i);
					OutputDebugStringA(ch);


					//pModelNode->emissiveColor = Vec3(0.5, 0.5, 0.8);

					char buf[64];
					//sprintf_s(buf, "Group=%s step=0.1", pModelNode->m_name.c_str());

					//std::string paramName = std::to_string(i++);

					TwAddVarRW(pCore->barHierarchy, "translate x", TW_TYPE_FLOAT, &pModelNode->local.translation.x, buf);
					TwAddVarRW(pCore->barHierarchy, "translate y", TW_TYPE_FLOAT, &pModelNode->local.translation.y, buf);
					TwAddVarRW(pCore->barHierarchy, "translate z", TW_TYPE_FLOAT, &pModelNode->local.translation.z, buf);

					TwAddVarRW(pCore->barHierarchy, "scale x", TW_TYPE_FLOAT, &pModelNode->local.scale.x, buf);
					TwAddVarRW(pCore->barHierarchy, "scale y", TW_TYPE_FLOAT, &pModelNode->local.scale.y, buf);
					TwAddVarRW(pCore->barHierarchy, "scale z", TW_TYPE_FLOAT, &pModelNode->local.scale.z, buf);

					TwAddVarRW(pCore->barHierarchy, "rotate x", TW_TYPE_FLOAT, &pModelNode->local.rotation.x, buf);
					TwAddVarRW(pCore->barHierarchy, "rotate y", TW_TYPE_FLOAT, &pModelNode->local.rotation.y, buf);
					TwAddVarRW(pCore->barHierarchy, "rotate z", TW_TYPE_FLOAT, &pModelNode->local.rotation.z, buf);
					/*float asd = 0;
					TwAddVarRW(pCore->barHierarchy, "rotate x", TW_TYPE_FLOAT, &asd, buf);*/
				}
				else
				{
					//pModelNode->emissiveColor = Vec3(0, 0, 0);
				}
			}
		}
	}
}

Core::Core(Window* window, Camera* camera, Shader shader)
{
	pCore = this;

	globalCamera = camera;
	this->screen = window;
	this->camera = camera;	
	scene = new Scene(&shader);
	isCursorEnabled = window->isCursorEnabled;

	glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
	glfwSetMouseButtonCallback(window->getWindow(), mouse_button);

	barHierarchy = TwNewBar("hierarchy");

	glm::mat4 transform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shader.programHandle, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

Core::~Core()
{
}

void Core::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_W);
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_S);
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_A);
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_D);
	//glfwGetCursorPos(window, &xpos, &ypos);
}

void Core::update(GLuint programHandle, Shader shader)
{
	GLFWwindow* window = screen->getWindow();
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera->update(programHandle, screen, deltaTime);
		shader.use();

		scene->Render();

		TwDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//void Core::processMouseMovement()
//{
//	GLfloat xoffset = xpos - lastX;
//	GLfloat yoffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;
//	camera->processMouseMovement(xoffset, yoffset, true);
//}