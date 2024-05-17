void WebGL2RenderingContextBase::uniformMatrix3x2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix3x2fv", location,
                                       transpose, value.data(), value.size(), 6,
                                       src_offset, src_length))
    return;
  ContextGL()->UniformMatrix3x2fv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) / 6, transpose,
      value.data() + src_offset);
}
