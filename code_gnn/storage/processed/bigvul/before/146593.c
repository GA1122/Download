void WebGLRenderingContextBase::vertexAttrib2fv(GLuint index,
                                                const Vector<GLfloat>& v) {
  if (isContextLost())
    return;
  if (v.size() < 2) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttrib2fv", "invalid array");
    return;
  }
  ContextGL()->VertexAttrib2fv(index, v.data());
  SetVertexAttribType(index, kFloat32ArrayType);
}
