void WebGLRenderingContextBase::uniform3iv(const WebGLUniformLocation* location,
                                           const FlexibleInt32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Int32Array>(
                             "uniform3iv", location, v, 3, 0, v.length()))
    return;

  ContextGL()->Uniform3iv(location->Location(), v.length() / 3,
                          v.DataMaybeOnStack());
}
