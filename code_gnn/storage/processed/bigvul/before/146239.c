void WebGL2RenderingContextBase::vertexAttribI4iv(
    GLuint index,
    MaybeShared<const DOMInt32Array> v) {
  if (isContextLost())
    return;
  if (!v.View() || v.View()->length() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttribI4iv", "invalid array");
    return;
  }
  ContextGL()->VertexAttribI4iv(index, v.View()->DataMaybeShared());
  SetVertexAttribType(index, kInt32ArrayType);
}
