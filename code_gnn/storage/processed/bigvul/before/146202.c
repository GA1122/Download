void WebGL2RenderingContextBase::uniform4fv(
    const WebGLUniformLocation* location,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform4fv", location, v.data(), v.size(), 4,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform4fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) >> 2,
      v.data() + src_offset);
}
