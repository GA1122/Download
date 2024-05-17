void WebGL2RenderingContextBase::vertexAttribI4uiv(GLuint index,
                                                   const Vector<GLuint>& v) {
  if (isContextLost())
    return;
  if (v.size() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttribI4uiv", "invalid array");
    return;
  }
  ContextGL()->VertexAttribI4uiv(index, v.data());
  SetVertexAttribType(index, kUint32ArrayType);
}
