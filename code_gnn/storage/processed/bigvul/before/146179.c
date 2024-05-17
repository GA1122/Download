void WebGL2RenderingContextBase::uniform1uiv(
    const WebGLUniformLocation* location,
    Vector<GLuint>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform1uiv", location, value.data(),
                                 value.size(), 1, src_offset, src_length))
    return;

  ContextGL()->Uniform1uiv(
      location->Location(),
      src_length ? src_length : (value.size() - src_offset),
      value.data() + src_offset);
}
