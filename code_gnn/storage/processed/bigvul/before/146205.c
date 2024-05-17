void WebGL2RenderingContextBase::uniform4iv(
    const WebGLUniformLocation* location,
    const FlexibleInt32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Int32Array>("uniform4iv", location, v, 4,
                                                  src_offset, src_length))
    return;

  ContextGL()->Uniform4iv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 2,
      v.DataMaybeOnStack() + src_offset);
}
