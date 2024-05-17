void WebGL2RenderingContextBase::uniform2iv(
    const WebGLUniformLocation* location,
    const FlexibleInt32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Int32Array>("uniform2iv", location, v, 2,
                                                  src_offset, src_length))
    return;

  ContextGL()->Uniform2iv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 1,
      v.DataMaybeOnStack() + src_offset);
}
