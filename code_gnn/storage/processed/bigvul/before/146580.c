void WebGLRenderingContextBase::uniformMatrix2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix2fv", location, transpose,
                                       v.View(), 4, 0, v.View()->length()))
    return;
  ContextGL()->UniformMatrix2fv(location->Location(), v.View()->length() >> 2,
                                transpose, v.View()->DataMaybeShared());
}
