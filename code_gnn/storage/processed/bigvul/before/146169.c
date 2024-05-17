void WebGL2RenderingContextBase::uniform1fv(
    const WebGLUniformLocation* location,
    const FlexibleFloat32ArrayView& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters<WTF::Float32Array>("uniform1fv", location, v,
                                                    1, src_offset, src_length))
    return;

  ContextGL()->Uniform1fv(location->Location(),
                          src_length ? src_length : (v.length() - src_offset),
                          v.DataMaybeOnStack() + src_offset);
}
