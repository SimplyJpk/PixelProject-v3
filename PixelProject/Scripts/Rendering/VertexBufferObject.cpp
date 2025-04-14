#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(const VBOShape shape) : _vao(0), _vbo(0), _ebo(0), _num_indices(0)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	if (shape == VBOShape::Square) {
		static constexpr GLfloat SQUARE_VERTICES[] = {
			// position      // texture coordinates
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		static constexpr GLuint SQUARE_INDICES[] = {
			0, 1, 2,
			0, 2, 3
		};

		SetVertexData(SQUARE_VERTICES, sizeof(SQUARE_VERTICES));
		SetIndexData(SQUARE_INDICES, sizeof(SQUARE_INDICES));
		_num_indices = 6;
	}
	else if (shape == VBOShape::Triangle) {
		static constexpr GLfloat TRIANGLE_VERTICES[] = {
			// position      // texture coordinates
			0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		static constexpr GLuint TRIANGLE_INDICIES[] = {
			0, 1, 2
		};

		SetVertexData(TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES));
		SetIndexData(TRIANGLE_INDICIES, sizeof(TRIANGLE_INDICIES));
		_num_indices = 3;
	}
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void VertexBufferObject::SetVertexData(const GLfloat* vertices, GLsizeiptr size)
{
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void VertexBufferObject::SetIndexData(const GLuint* indices, GLsizeiptr size) const
{
	glBindVertexArray(_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void VertexBufferObject::Draw() const
{
	glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
}

void VertexBufferObject::Bind() const
{
	glBindVertexArray(_vao);
}

void VertexBufferObject::Unbind()
{
	glBindVertexArray(0);
}