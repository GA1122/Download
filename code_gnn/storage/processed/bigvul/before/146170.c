void WebGL2RenderingContextBase::uniform1fv(
    const WebGLUniformLocation* location,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform1fv", location, v.data(), v.size(), 1,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform1fv(location->Location(),
                          src_length ? src_length : (v.size() - src_offset),
                          v.data() + src_offset);
}
