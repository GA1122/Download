void WebGLRenderingContextBase::uniform2fv(const WebGLUniformLocation* location,
                                           const FlexibleFloat32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Float32Array>(
                             "uniform2fv", location, v, 2, 0, v.length()))
    return;

  ContextGL()->Uniform2fv(location->Location(), v.length() >> 1,
                          v.DataMaybeOnStack());
}
