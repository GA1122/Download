void WebGL2RenderingContextBase::uniform3fv(
    const WebGLUniformLocation* location,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform3fv", location, v.data(), v.size(), 3,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform3fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) / 3,
      v.data() + src_offset);
}
