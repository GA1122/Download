void WebGL2RenderingContextBase::uniform2fv(
    const WebGLUniformLocation* location,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform2fv", location, v.data(), v.size(), 2,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform2fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) >> 1,
      v.data() + src_offset);
}
