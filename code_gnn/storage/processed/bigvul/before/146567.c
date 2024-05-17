void WebGLRenderingContextBase::uniform2iv(const WebGLUniformLocation* location,
                                           Vector<GLint>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform2iv", location, v.data(), v.size(), 2,
                                 0, v.size()))
    return;

  ContextGL()->Uniform2iv(location->Location(), v.size() >> 1, v.data());
}
