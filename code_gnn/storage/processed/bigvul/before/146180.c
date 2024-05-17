void WebGL2RenderingContextBase::uniform2fv(
    const WebGLUniformLocation* location,
    const FlexibleFloat32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Float32Array>("uniform2fv", location, v,
                                                    2, src_offset, src_length))
    return;

  ContextGL()->Uniform2fv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 1,
      v.DataMaybeOnStack() + src_offset);
}
