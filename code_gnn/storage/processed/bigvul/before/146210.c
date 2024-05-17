void WebGL2RenderingContextBase::uniform4uiv(
    const WebGLUniformLocation* location,
    const FlexibleUint32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Uint32Array>("uniform4uiv", location, v,
                                                   4, src_offset, src_length))
    return;

  ContextGL()->Uniform4uiv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 2,
      v.DataMaybeOnStack() + src_offset);
}
