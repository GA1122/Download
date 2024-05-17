void WebGLRenderingContextBase::vertexAttrib3f(GLuint index,
                                               GLfloat v0,
                                               GLfloat v1,
                                               GLfloat v2) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttrib3f(index, v0, v1, v2);
  SetVertexAttribType(index, kFloat32ArrayType);
}
