void WebGLRenderingContextBase::vertexAttrib4fv(
    GLuint index,
    MaybeShared<const DOMFloat32Array> v) {
  if (isContextLost())
    return;
  if (!v.View() || v.View()->length() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttrib4fv", "invalid array");
    return;
  }
  ContextGL()->VertexAttrib4fv(index, v.View()->DataMaybeShared());
  SetVertexAttribType(index, kFloat32ArrayType);
}
