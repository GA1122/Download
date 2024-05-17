void WebGLRenderingContextBase::uniform3iv(const WebGLUniformLocation* location,
                                           Vector<GLint>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform3iv", location, v.data(), v.size(), 3,
                                 0, v.size()))
    return;

  ContextGL()->Uniform3iv(location->Location(), v.size() / 3, v.data());
}
