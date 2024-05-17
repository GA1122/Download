void WebGLRenderingContextBase::uniformMatrix4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformMatrixParameters("uniformMatrix4fv", location, transpose,
                                       v.data(), v.size(), 16, 0, v.size()))
    return;
  ContextGL()->UniformMatrix4fv(location->Location(), v.size() >> 4, transpose,
                                v.data());
}
