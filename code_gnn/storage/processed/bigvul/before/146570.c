void WebGLRenderingContextBase::uniform3fv(const WebGLUniformLocation* location,
                                           Vector<GLfloat>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform3fv", location, v.data(), v.size(), 3,
                                 0, v.size()))
    return;

  ContextGL()->Uniform3fv(location->Location(), v.size() / 3, v.data());
}
