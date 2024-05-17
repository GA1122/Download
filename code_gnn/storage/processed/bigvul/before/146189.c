void WebGL2RenderingContextBase::uniform2uiv(
    const WebGLUniformLocation* location,
    Vector<GLuint>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform2uiv", location, value.data(),
                                 value.size(), 2, src_offset, src_length))
    return;

  ContextGL()->Uniform2uiv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) >> 1,
      value.data() + src_offset);
}
