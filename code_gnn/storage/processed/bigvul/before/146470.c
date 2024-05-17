WebGLFramebuffer* WebGLRenderingContextBase::createFramebuffer() {
  if (isContextLost())
    return nullptr;
  return WebGLFramebuffer::Create(this);
}
