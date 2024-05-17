void WebGL2RenderingContextBase::vertexAttribI4iv(GLuint index,
                                                  const Vector<GLint>& v) {
  if (isContextLost())
    return;
  if (v.size() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttribI4iv", "invalid array");
    return;
  }
  ContextGL()->VertexAttribI4iv(index, v.data());
  SetVertexAttribType(index, kInt32ArrayType);
}
