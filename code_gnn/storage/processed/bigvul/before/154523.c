GLES2DecoderPassthroughImpl::EmulatedDefaultFramebuffer::SetColorBuffer(
    std::unique_ptr<EmulatedColorBuffer> new_color_buffer) {
  DCHECK(color_texture != nullptr && new_color_buffer != nullptr);
  DCHECK(color_texture->size == new_color_buffer->size);
  std::unique_ptr<EmulatedColorBuffer> old_buffer(std::move(color_texture));
  color_texture = std::move(new_color_buffer);

  ScopedFramebufferBindingReset scoped_fbo_reset(
      api, supports_separate_fbo_bindings);
  api->glBindFramebufferEXTFn(GL_FRAMEBUFFER, framebuffer_service_id);
  api->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D,
                                   color_texture->texture->service_id(), 0);

  return old_buffer;
}
