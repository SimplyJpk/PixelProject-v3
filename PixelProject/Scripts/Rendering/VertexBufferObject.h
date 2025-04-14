#pragma once

#include <GL/glew.h>
#include <unordered_map>

enum class VBOShape
{
	None,
	Square,
	Triangle
};

class VertexBufferObject
{
 public:
	VertexBufferObject(VBOShape shape = VBOShape::None);
	~VertexBufferObject();

	void SetVertexData(const GLfloat* vertices, GLsizeiptr size);
	void SetIndexData(const GLuint* indices, GLsizeiptr size) const;

	void Bind() const;
	static void Unbind();

	void Draw() const;

 protected:
	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;
	GLsizei _num_indices;

 private:

	//   static constexpr GLfloat squareVertices[] = {
	//      // positions         // texture coordinates
	//      0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
	//      0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
	//     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
	//   };
	//   static constexpr GLuint squareIndices[] = {
	//      0, 1, 3, // first triangle
	//      1, 2, 3  // second triangle
	//   };
	//
	//   static constexpr GLfloat triangleVertices[] = {
	//      // positions          // texture coordinates
	//      0.0f,  0.5f, 0.0f,   0.5f, 1.0f,
	//      0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
	//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f
	// };
	//
	//   static constexpr GLuint triangleVIndices[] = {
	//      0, 1, 2
	//  };
	//
};