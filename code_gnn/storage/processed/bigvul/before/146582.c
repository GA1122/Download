void WebGLRenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix3fv", location, transpose,
                                       v.View(), 9, 0, v.View()->length()))
    return;
  ContextGL()->UniformMatrix3fv(location->Location(), v.View()->length() / 9,
                                transpose, v.View()->DataMaybeShared());
}
