void WebGLRenderingContextBase::deleteRenderbuffer(
    WebGLRenderbuffer* renderbuffer) {
  if (!DeleteObject(renderbuffer))
    return;
  if (renderbuffer == renderbuffer_binding_) {
    renderbuffer_binding_ = nullptr;
  }
  if (framebuffer_binding_)
    framebuffer_binding_->RemoveAttachmentFromBoundFramebuffer(GL_FRAMEBUFFER,
                                                               renderbuffer);
  if (GetFramebufferBinding(GL_READ_FRAMEBUFFER))
    GetFramebufferBinding(GL_READ_FRAMEBUFFER)
        ->RemoveAttachmentFromBoundFramebuffer(GL_READ_FRAMEBUFFER,
                                               renderbuffer);
}
