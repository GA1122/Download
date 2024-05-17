void WebGL2RenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix3fv", location, transpose,
                                       v.View(), 9, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix3fv(
      location->Location(),
      (src_length ? src_length : (v.View()->length() - src_offset)) / 9,
      transpose, v.View()->DataMaybeShared() + src_offset);
}
