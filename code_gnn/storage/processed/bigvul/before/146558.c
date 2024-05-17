void WebGLRenderingContextBase::uniform1fv(const WebGLUniformLocation* location,
                                           const FlexibleFloat32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Float32Array>(
                             "uniform1fv", location, v, 1, 0, v.length()))
    return;

  ContextGL()->Uniform1fv(location->Location(), v.length(),
                          v.DataMaybeOnStack());
}
