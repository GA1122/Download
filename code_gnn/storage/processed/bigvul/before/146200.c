void WebGL2RenderingContextBase::uniform3uiv(
    const WebGLUniformLocation* location,
    Vector<GLuint>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform3uiv", location, value.data(),
                                 value.size(), 3, src_offset, src_length))
    return;

  ContextGL()->Uniform3uiv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) / 3,
      value.data() + src_offset);
}
