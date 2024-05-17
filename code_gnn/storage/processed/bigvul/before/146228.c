void WebGL2RenderingContextBase::uniformMatrix3x4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix3x4fv", location,
                                       transpose, value.data(), value.size(),
                                       12, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix3x4fv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) / 12, transpose,
      value.data() + src_offset);
}
