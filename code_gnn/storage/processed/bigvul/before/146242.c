void WebGL2RenderingContextBase::vertexAttribI4uiv(
    GLuint index,
    MaybeShared<const DOMUint32Array> v) {
  if (isContextLost())
    return;
  if (!v.View() || v.View()->length() < 4) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttribI4uiv", "invalid array");
    return;
  }
  ContextGL()->VertexAttribI4uiv(index, v.View()->DataMaybeShared());
  SetVertexAttribType(index, kUint32ArrayType);
}
