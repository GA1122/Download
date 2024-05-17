void WebGL2RenderingContextBase::uniform1iv(
    const WebGLUniformLocation* location,
    Vector<GLint>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform1iv", location, v.data(), v.size(), 1,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform1iv(location->Location(),
                          src_length ? src_length : (v.size() - src_offset),
                          v.data() + src_offset);
}
