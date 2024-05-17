void WebGL2RenderingContextBase::uniformMatrix2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v,
    GLuint src_offset,
    GLuint src_length) {
  if (isContextLost() || !ValidateUniformMatrixParameters(
                             "uniformMatrix2fv", location, transpose, v.data(),
                             v.size(), 4, src_offset, src_length))
    return;
  ContextGL()->UniformMatrix2fv(
      location->Location(),
      (src_length ? src_length : (v.size() - src_offset)) >> 2, transpose,
      v.data() + src_offset);
}
