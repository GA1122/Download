void WebGL2RenderingContextBase::uniform1iv(
    const WebGLUniformLocation* location,
    const FlexibleInt32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Int32Array>("uniform1iv", location, v, 1,
                                                  src_offset, src_length))
    return;

  ContextGL()->Uniform1iv(location->Location(),
                          src_length ? src_length : (v.length() - src_offset),
                          v.DataMaybeOnStack() + src_offset);
}
