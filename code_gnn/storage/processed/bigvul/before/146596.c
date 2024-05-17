void WebGLRenderingContextBase::vertexAttrib3fv(GLuint index,
                                                const Vector<GLfloat>& v) {
  if (isContextLost())
    return;
  if (v.size() < 3) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttrib3fv", "invalid array");
    return;
  }
  ContextGL()->VertexAttrib3fv(index, v.data());
  SetVertexAttribType(index, kFloat32ArrayType);
}
