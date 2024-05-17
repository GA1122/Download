void WebGLRenderingContextBase::uniform1iv(const WebGLUniformLocation* location,
                                           Vector<GLint>& v) {
  if (isContextLost() ||
      !ValidateUniformParameters("uniform1iv", location, v.data(), v.size(), 1,
                                 0, v.size()))
    return;

  ContextGL()->Uniform1iv(location->Location(), v.size(), v.data());
}
