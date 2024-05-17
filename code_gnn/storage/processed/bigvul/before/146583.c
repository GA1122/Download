void WebGLRenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix3fv", location, transpose,
                                       v.data(), v.size(), 9, 0, v.size()))
    return;
  ContextGL()->UniformMatrix3fv(location->Location(), v.size() / 9, transpose,
                                v.data());
}
