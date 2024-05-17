WebGLRenderbuffer* WebGLRenderingContextBase::createRenderbuffer() {
  if (isContextLost())
    return nullptr;
  return WebGLRenderbuffer::Create(this);
}
