void WebGLRenderingContextBase::vertexAttrib3fv(
    GLuint index,
    MaybeShared<const DOMFloat32Array> v) {
  if (isContextLost())
    return;
  if (!v.View() || v.View()->length() < 3) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttrib3fv", "invalid array");
    return;
  }
  ContextGL()->VertexAttrib3fv(index, v.View()->DataMaybeShared());
  SetVertexAttribType(index, kFloat32ArrayType);
}
