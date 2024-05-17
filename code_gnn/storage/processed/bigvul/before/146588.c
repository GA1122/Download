void WebGLRenderingContextBase::vertexAttrib1f(GLuint index, GLfloat v0) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttrib1f(index, v0);
  SetVertexAttribType(index, kFloat32ArrayType);
}
