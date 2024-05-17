void WebGLRenderingContextBase::vertexAttrib2f(GLuint index,
                                               GLfloat v0,
                                               GLfloat v1) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttrib2f(index, v0, v1);
  SetVertexAttribType(index, kFloat32ArrayType);
}
