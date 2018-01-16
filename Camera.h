#include "glm/glm.hpp"

class GLFWwindow;

class Camera
{
public:
	Camera();

	void SetAspectRatio(float newAspectRatio);

	glm::vec3 GetPosition() const { return position; }
	void SetPosition(glm::vec3 positionCam) { position = positionCam; }

	void processInput(GLFWwindow* window, float dt);

	void UpdatePerspectiveMatrix();

	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }

	void UpdateOrientation();

	glm::mat4 view;
	glm::mat4 projection;

private:
	glm::vec3 position;
	glm::vec3 upVector;
	glm::vec3 frontVector;

	float aspectRatio;

	// orientation
	float yaw;
	float pitch;

};