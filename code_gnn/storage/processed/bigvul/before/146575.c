void WebGLRenderingContextBase::uniform4fv(const WebGLUniformLocation* location,
                                           const FlexibleFloat32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Float32Array>(
                             "uniform4fv", location, v, 4, 0, v.length()))
    return;

  ContextGL()->Uniform4fv(location->Location(), v.length() >> 2,
                          v.DataMaybeOnStack());
}
