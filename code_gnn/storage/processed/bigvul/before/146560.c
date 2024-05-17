void WebGLRenderingContextBase::uniform1iv(const WebGLUniformLocation* location,
                                           const FlexibleInt32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Int32Array>(
                             "uniform1iv", location, v, 1, 0, v.length()))
    return;

  ContextGL()->Uniform1iv(location->Location(), v.length(),
                          v.DataMaybeOnStack());
}
