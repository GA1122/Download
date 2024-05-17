void WebGL2RenderingContextBase::uniformMatrix4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix4fv", location, transpose, v.data(),
                             v.size(), 16, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix4fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) >> 4, transpose,
      v.data() + src_offset);
}
