#include "Camera.h"
#include <glm/ext.hpp>

void Camera::Update(float delta)
{

}

void Camera::SetOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
	_dirty = true;
}

void Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
	_projection_matrix = glm::perspective(fov, aspectRatio, near, far);
	_dirty = true;
}

void Camera::SetPosition(const glm::vec3& position)
{
	_position = position;
	_world_transform[3] = glm::vec4(_position, 1);
	_view_matrix = glm::inverse(_world_transform);
	_dirty = true;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return _view_matrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return _projection_matrix;
}

const glm::mat4& Camera::GetViewProjectionMatrix() const
{
	return _view_projection_matrix;
}

const glm::vec3& Camera::GetPosition() const
{
	return _position;
}

const glm::vec4* Camera::GetFrustumPlanes() const
{
	return _frustum;
}

glm::vec3 Camera::Row(int row)
{
	return glm::vec3(_world_transform[row]);
}

void Camera::GenerateFrustum(const bool normalized)
{
	_view_projection_matrix = _projection_matrix * _view_matrix;
	glm::mat4 matrix = _view_projection_matrix;
	// Left clipping plane
	_frustum[0][0] = matrix[0][3] + matrix[0][0];
	_frustum[0][1] = matrix[1][3] + matrix[1][0];
	_frustum[0][2] = matrix[2][3] + matrix[2][0];
	_frustum[0][3] = matrix[3][3] + matrix[3][0];
	// Right clipping plane
	_frustum[1][0] = matrix[0][3] - matrix[0][0];
	_frustum[1][1] = matrix[1][3] - matrix[1][0];
	_frustum[1][2] = matrix[2][3] - matrix[2][0];
	_frustum[1][3] = matrix[3][3] - matrix[3][0];
	// Top clipping plane
	_frustum[2][0] = matrix[0][3] - matrix[0][1];
	_frustum[2][1] = matrix[1][3] - matrix[1][1];
	_frustum[2][2] = matrix[2][3] - matrix[2][1];
	_frustum[2][3] = matrix[3][3] - matrix[3][1];
	// Bottom clipping plane
	_frustum[3][0] = matrix[0][3] + matrix[0][1];
	_frustum[3][1] = matrix[1][3] + matrix[1][1];
	_frustum[3][2] = matrix[2][3] + matrix[2][1];
	_frustum[3][3] = matrix[3][3] + matrix[3][1];
	// Near clipping plane
	_frustum[4][0] = matrix[0][2];
	_frustum[4][1] = matrix[1][2];
	_frustum[4][2] = matrix[2][2];
	_frustum[4][3] = matrix[3][2];
	// Far clipping plane
	_frustum[5][0] = matrix[0][3] - matrix[0][2];
	_frustum[5][1] = matrix[1][3] - matrix[1][2];
	_frustum[5][2] = matrix[2][3] - matrix[2][2];
	_frustum[5][3] = matrix[3][3] - matrix[3][2];
	// Normalize the plane equations, if requested
	if (normalized) {
		for (int i = 0; i < 6; i++) {
			float t = sqrt(
				_frustum[i][0] * _frustum[i][0] + _frustum[i][1] * _frustum[i][1] + _frustum[i][2] * _frustum[i][2]);
			_frustum[i][0] /= t;
			_frustum[i][1] /= t;
			_frustum[i][2] /= t;
			_frustum[i][3] /= t;
		}
	}

}
const float Camera::GetZoom() const
{
	return _zoom;
}

