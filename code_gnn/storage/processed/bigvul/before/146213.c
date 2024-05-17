void WebGL2RenderingContextBase::uniformMatrix2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix2fv", location, transpose,
                                       v.View(), 4, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix2fv(
      location->Location(),
      (src_length ? src_length : (v.View()->length() - src_offset)) >> 2,
      transpose, v.View()->DataMaybeShared() + src_offset);
}
