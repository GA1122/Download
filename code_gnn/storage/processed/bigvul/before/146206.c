void WebGL2RenderingContextBase::uniform4iv(
    const WebGLUniformLocation* location,
    Vector<GLint>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform4iv", location, v.data(), v.size(), 4,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform4iv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) >> 2,
      v.data() + src_offset);
}
