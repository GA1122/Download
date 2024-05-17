void WebGL2RenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix3fv", location, transpose, v.data(),
                             v.size(), 9, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix3fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) / 9, transpose,
      v.data() + src_offset);
}
