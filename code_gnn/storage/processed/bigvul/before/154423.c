void GLES2DecoderPassthroughImpl::EmulatedDefaultFramebuffer::Blit(
    EmulatedColorBuffer* target) {
  DCHECK(target != nullptr);
  DCHECK(target->size == size);

  ScopedFramebufferBindingReset scoped_fbo_reset(
      api, supports_separate_fbo_bindings);

  api->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER, framebuffer_service_id);

  GLuint temp_fbo;
  api->glGenFramebuffersEXTFn(1, &temp_fbo);
  api->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, temp_fbo);
  api->glFramebufferTexture2DEXTFn(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, target->texture->service_id(),
                                   0);

  api->glBlitFramebufferFn(0, 0, size.width(), size.height(), 0, 0,
                           target->size.width(), target->size.height(),
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);

  api->glDeleteFramebuffersEXTFn(1, &temp_fbo);
}
