void WebGL2RenderingContextBase::uniformMatrix4x3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& value,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix4x3fv", location,
                                       transpose, value.data(), value.size(),
                                       12, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix4x3fv(
      location->Location(),
      (src_length ? src_length : (value.size() - src_offset)) / 12, transpose,
      value.data() + src_offset);
}
