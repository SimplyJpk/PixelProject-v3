#pragma once
#include <glm/glm.hpp>

class Camera
{
 public:
	Camera() = default;
	virtual ~Camera() = default;

	virtual void Update(float delta);

	// Basic camera, Orthographic and Perspective with a zoom factor with the normal matrixes
	void SetOrthographic(float left, float right, float bottom, float top, float near, float far);
	void SetPerspective(float fov, float aspectRatio, float near, float far);
	void SetZoom(float zoom);

	void SetPosition(const glm::vec3& position);

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewProjectionMatrix() const;

	const glm::vec3& GetPosition() const;
	const glm::vec4* GetFrustumPlanes() const;

	const float GetZoom() const;

 protected:
	glm::vec3 Row(int row);

	void GenerateFrustum(bool normalized = true);
	glm::vec4 _frustum[6];

	glm::mat4 _world_transform;
	glm::mat4 _view_matrix;
	glm::mat4 _projection_matrix;
	glm::mat4 _view_projection_matrix;

	glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
	float _zoom = 1.0f;

	bool _dirty = true;

};
