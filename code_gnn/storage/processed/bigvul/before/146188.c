void WebGL2RenderingContextBase::uniform2uiv(
    const WebGLUniformLocation* location,
    const FlexibleUint32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Uint32Array>("uniform2uiv", location, v,
                                                   2, src_offset, src_length))
    return;

  ContextGL()->Uniform2uiv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 1,
      v.DataMaybeOnStack() + src_offset);
}
