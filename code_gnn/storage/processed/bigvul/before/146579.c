void WebGLRenderingContextBase::uniform4iv(const WebGLUniformLocation* location,
                                           Vector<GLint>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform4iv", location, v.data(), v.size(), 4,
                                 0, v.size()))
    return;

  ContextGL()->Uniform4iv(location->Location(), v.size() >> 2, v.data());
}
