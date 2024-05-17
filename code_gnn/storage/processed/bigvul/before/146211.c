void WebGL2RenderingContextBase::uniform4uiv(
    const WebGLUniformLocation* location,
    Vector<GLuint>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform4uiv", location, value.data(),
                                 value.size(), 4, src_offset, src_length))
    return;

  ContextGL()->Uniform4uiv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) >> 2,
      value.data() + src_offset);
}
