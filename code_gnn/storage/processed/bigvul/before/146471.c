WebGLProgram* WebGLRenderingContextBase::createProgram() {
  if (isContextLost())
    return nullptr;
  return WebGLProgram::Create(this);
}
