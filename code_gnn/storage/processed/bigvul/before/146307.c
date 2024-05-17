WebGLFramebuffer* WebGLRenderingContextBase::GetFramebufferBinding(
    GLenum target) {
  if (target == GL_FRAMEBUFFER)
    return framebuffer_binding_.Get();
  return nullptr;
}
