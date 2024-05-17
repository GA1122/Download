void WebGLRenderingContextBase::vertexAttrib4f(GLuint index,
                                               GLfloat v0,
                                               GLfloat v1,
                                               GLfloat v2,
                                               GLfloat v3) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttrib4f(index, v0, v1, v2, v3);
  SetVertexAttribType(index, kFloat32ArrayType);
}
