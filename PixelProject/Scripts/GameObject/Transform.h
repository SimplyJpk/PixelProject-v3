#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Macros/InlineReturn.h"

class Transform
{
 public:
	// Constructors
	Transform();
	Transform(const glm::vec3& position);
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	// Getters and Setters
	const glm::vec3& GetPosition() const
	INLINE_RETURN(_position)
	void SetPosition(const glm::vec3& position);

	const glm::vec3& GetRotation() const
	INLINE_RETURN(_rotation)
	void SetRotation(const glm::vec3& rotation);

	const glm::vec3& GetScale() const
	INLINE_RETURN(_scale)
	void SetScale(const glm::vec3& scale);

	// Helper Methods
	glm::mat4 GetModelMatrix() const;

	glm::vec4& GetBounds() const;

 protected:
	inline void UpdateBounds() const;

 private:
	void UpdateModelMatrix() const;

	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;

	mutable glm::vec4 _bounding_box;
	mutable bool _is_bounding_box_is_dirty;

	mutable glm::mat4 _model_matrix;
	mutable bool _is_model_matrix_is_dirty;
};
