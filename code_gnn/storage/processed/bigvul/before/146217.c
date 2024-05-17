void WebGL2RenderingContextBase::uniformMatrix2x3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix2x3fv", location, transpose,
                             value.View(), 6, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix2x3fv(
      location->Location(),
      (src_length ? src_length : (value.View()->length() - src_offset)) / 6,
      transpose, value.View()->DataMaybeShared() + src_offset);
}
