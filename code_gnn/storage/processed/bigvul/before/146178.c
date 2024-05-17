void WebGL2RenderingContextBase::uniform1uiv(
    const WebGLUniformLocation* location,
    const FlexibleUint32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Uint32Array>("uniform1uiv", location, v,
                                                   1, src_offset, src_length))
    return;

  ContextGL()->Uniform1uiv(location->Location(),
                           src_length ? src_length : (v.length() - src_offset),
                           v.DataMaybeOnStack() + src_offset);
}
