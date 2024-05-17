void WebGL2RenderingContextBase::uniform4fv(
    const WebGLUniformLocation* location,
    const FlexibleFloat32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Float32Array>("uniform4fv", location, v,
                                                    4, src_offset, src_length))
    return;

  ContextGL()->Uniform4fv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) >> 2,
      v.DataMaybeOnStack() + src_offset);
}
