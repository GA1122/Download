void WebGLRenderingContextBase::uniform4iv(const WebGLUniformLocation* location,
                                           const FlexibleInt32ArrayView& v) {
  if (isContextLost() || !ValidateUniformParameters<WTF::Int32Array>(
                             "uniform4iv", location, v, 4, 0, v.length()))
    return;

  ContextGL()->Uniform4iv(location->Location(), v.length() >> 2,
                          v.DataMaybeOnStack());
}
