void WebGLRenderingContextBase::framebufferTexture2D(GLenum target,
                                                     GLenum attachment,
                                                     GLenum textarget,
                                                     WebGLTexture* texture,
                                                     GLint level) {
  if (isContextLost() || !ValidateFramebufferFuncParameters(
                             "framebufferTexture2D", target, attachment))
    return;
  if (!ValidateNullableWebGLObject("framebufferTexture2D", texture))
    return;

  WebGLFramebuffer* framebuffer_binding = GetFramebufferBinding(target);
  if (!framebuffer_binding || !framebuffer_binding->Object()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "framebufferTexture2D",
                      "no framebuffer bound");
    return;
  }
  if (framebuffer_binding && framebuffer_binding->Opaque()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "framebufferTexture2D",
                      "opaque framebuffer bound");
    return;
  }
  framebuffer_binding->SetAttachmentForBoundFramebuffer(
      target, attachment, textarget, texture, level, 0, 0);
  ApplyStencilTest();
}
