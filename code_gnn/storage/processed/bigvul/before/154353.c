ScopedFramebufferCopyBinder::ScopedFramebufferCopyBinder(
    GLES2DecoderImpl* decoder,
    GLint x,
    GLint y,
    GLint width,
    GLint height)
    : decoder_(decoder) {
  const Framebuffer::Attachment* attachment =
      decoder->framebuffer_state_.bound_read_framebuffer.get()
          ->GetReadBufferAttachment();
  DCHECK(attachment);
  auto* api = decoder_->api();
  api->glGenTexturesFn(1, &temp_texture_);

  ScopedTextureBinder texture_binder(&decoder->state_,
                                     decoder->error_state_.get(), temp_texture_,
                                     GL_TEXTURE_2D);
  if (width == 0 || height == 0) {
    api->glCopyTexImage2DFn(GL_TEXTURE_2D, 0, attachment->internal_format(), 0,
                            0, attachment->width(), attachment->height(), 0);
  } else {
    api->glCopyTexImage2DFn(GL_TEXTURE_2D, 0, attachment->internal_format(), x,
                            y, width, height, 0);
  }

  api->glGenFramebuffersEXTFn(1, &temp_framebuffer_);
  framebuffer_binder_ =
      std::make_unique<ScopedFramebufferBinder>(decoder, temp_framebuffer_);
  api->glFramebufferTexture2DEXTFn(GL_READ_FRAMEBUFFER_EXT,
                                   GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                   temp_texture_, 0);
  api->glReadBufferFn(GL_COLOR_ATTACHMENT0);
}
