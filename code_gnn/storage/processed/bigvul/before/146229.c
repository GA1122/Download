void WebGL2RenderingContextBase::uniformMatrix4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix4fv", location, transpose,
                                       v.View(), 16, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix4fv(
      location->Location(),
      (src_length ? src_length : (v.View()->length() - src_offset)) >> 4,
      transpose, v.View()->DataMaybeShared() + src_offset);
}
