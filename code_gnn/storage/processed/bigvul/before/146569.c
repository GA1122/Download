void WebGLRenderingContextBase::uniform3fv(const WebGLUniformLocation* location,
                                           const FlexibleFloat32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Float32Array>(
                             "uniform3fv", location, v, 3, 0, v.length()))
    return;

  ContextGL()->Uniform3fv(location->Location(), v.length() / 3,
                          v.DataMaybeOnStack());
}
