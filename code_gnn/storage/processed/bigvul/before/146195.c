void WebGL2RenderingContextBase::uniform3iv(
    const WebGLUniformLocation* location,
    Vector<GLint>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform3iv", location, v.data(), v.size(), 3,
                                 src_offset, src_length))
    return;

  ContextGL()->Uniform3iv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) / 3,
      v.data() + src_offset);
}
