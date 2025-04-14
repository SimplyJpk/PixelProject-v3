#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
	: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f)
{
	UpdateModelMatrix();
	UpdateBounds();
}

Transform::Transform(const glm::vec3& position)
	: _position(position), _rotation(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f)
{
	UpdateModelMatrix();
	UpdateBounds();
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: _position(position), _rotation(rotation), _scale(scale)
{
	UpdateModelMatrix();
	UpdateBounds();
}

void Transform::SetPosition(const glm::vec3& position)
{
	_position = position;
	_is_model_matrix_is_dirty = true;
	_is_bounding_box_is_dirty = true;
}

void Transform::SetRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
	_is_model_matrix_is_dirty = true;
}

void Transform::SetScale(const glm::vec3& scale)
{
	if (_scale == scale)
		return;
	_scale = scale;
	_is_model_matrix_is_dirty = true;
	_is_bounding_box_is_dirty = true;
}

glm::mat4 Transform::GetModelMatrix() const
{
	if (_is_model_matrix_is_dirty)
		UpdateModelMatrix();
	return _model_matrix;
}

void Transform::UpdateModelMatrix() const
{
	_model_matrix = glm::mat4(1.0f);
	_model_matrix = glm::translate(_model_matrix, _position);
	_model_matrix = glm::rotate(_model_matrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_model_matrix = glm::rotate(_model_matrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_model_matrix = glm::rotate(_model_matrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	_model_matrix = glm::scale(_model_matrix, _scale);

	_is_model_matrix_is_dirty = false;
}

void Transform::UpdateBounds() const
{
	float scaleX = (_scale.x / 2);
	float scaleY = (_scale.y / 2);
	_bounding_box.x = _position.x - scaleX; // Min-X
	_bounding_box.y = _position.y - scaleY; // Min-Y
	_bounding_box.z = _position.x + scaleX; // Max-X
	_bounding_box.w = _position.y + scaleY; // Max-Y
}

glm::vec4& Transform::GetBounds() const
{
	if (_is_bounding_box_is_dirty)
		UpdateBounds();
	return _bounding_box;
}

