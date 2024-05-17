void WebGLRenderingContextBase::uniformMatrix2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix2fv", location, transpose,
                                       v.data(), v.size(), 4, 0, v.size()))
    return;
  ContextGL()->UniformMatrix2fv(location->Location(), v.size() >> 2, transpose,
                                v.data());
}
