void WebGLRenderingContextBase::uniform2fv(const WebGLUniformLocation* location,
                                           Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform2fv", location, v.data(), v.size(), 2,
                                 0, v.size()))
    return;

  ContextGL()->Uniform2fv(location->Location(), v.size() >> 1, v.data());
}
