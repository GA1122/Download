void WebGLRenderingContextBase::uniform4fv(const WebGLUniformLocation* location,
                                           Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform4fv", location, v.data(), v.size(), 4,
                                 0, v.size()))
    return;

  ContextGL()->Uniform4fv(location->Location(), v.size() >> 2, v.data());
}
