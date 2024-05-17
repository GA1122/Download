void WebGL2RenderingContextBase::uniformMatrix3x2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix3x2fv", location, transpose,
                             value.View(), 6, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix3x2fv(
      location->Location(),
      (src_length ? src_length : (value.View()->length() - src_offset)) / 6,
      transpose, value.View()->DataMaybeShared() + src_offset);
}
