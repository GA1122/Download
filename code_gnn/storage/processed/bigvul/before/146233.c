void WebGL2RenderingContextBase::uniformMatrix4x2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix4x2fv", location, transpose,
                             value.View(), 8, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix4x2fv(
      location->Location(),
      (src_length ? src_length : (value.View()->length() - src_offset)) >> 3,
      transpose, value.View()->DataMaybeShared() + src_offset);
}
