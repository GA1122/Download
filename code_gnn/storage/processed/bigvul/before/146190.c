void WebGL2RenderingContextBase::uniform3fv(
    const WebGLUniformLocation* location,
    const FlexibleFloat32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Float32Array>("uniform3fv", location, v,
                                                    3, src_offset, src_length))
    return;

  ContextGL()->Uniform3fv(
      location->Location(),
      (src_length ? src_length : (v.length() - src_offset)) / 3,
      v.DataMaybeOnStack() + src_offset);
}
