void WebGLRenderingContextBase::vertexAttrib4fv(GLuint index,
                                                const Vector<GLfloat>& v) {
  if (isContextLost())
    return;
  if (v.size() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttrib4fv", "invalid array");
    return;
  }
  ContextGL()->VertexAttrib4fv(index, v.data());
  SetVertexAttribType(index, kFloat32ArrayType);
}
