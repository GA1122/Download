bool BackTexture::AllocateNativeGpuMemoryBuffer(const gfx::Size& size,
                                                GLenum format,
                                                bool zero) {
  if (!decoder_->GetContextGroup()
           ->image_factory()
           ->SupportsCreateAnonymousImage())
    return false;

  DCHECK(format == GL_RGB || format == GL_RGBA);
  bool is_cleared = false;
  gfx::BufferFormat buffer_format = gfx::BufferFormat::RGBA_8888;
  if (format == GL_RGB) {
#if defined(USE_OZONE)
    buffer_format = gfx::BufferFormat::BGRX_8888;
#else
    buffer_format = gfx::BufferFormat::RGBX_8888;
#endif
  }
  DCHECK_EQ(format, gpu::InternalFormatForGpuMemoryBufferFormat(buffer_format));
  scoped_refptr<gl::GLImage> image =
      decoder_->GetContextGroup()->image_factory()->CreateAnonymousImage(
          size, buffer_format, gfx::BufferUsage::SCANOUT, &is_cleared);
  if (!image || !image->BindTexImage(Target()))
    return false;

  image_ = image;
  decoder_->texture_manager()->SetLevelInfo(
      texture_ref_.get(), Target(), 0, image_->GetInternalFormat(),
      size.width(), size.height(), 1, 0,
      TextureManager::ExtractFormatFromStorageFormat(
          image_->GetInternalFormat()),
      GL_UNSIGNED_BYTE, gfx::Rect(size));
  decoder_->texture_manager()->SetLevelImage(texture_ref_.get(), Target(), 0,
                                             image_.get(), Texture::BOUND);

  bool needs_clear_for_rgb_emulation =
      !decoder_->offscreen_buffer_should_have_alpha_ &&
      decoder_->ChromiumImageNeedsRGBEmulation();
  if (!is_cleared || zero || needs_clear_for_rgb_emulation) {
    GLuint fbo;
    api()->glGenFramebuffersEXTFn(1, &fbo);
    {
      ScopedFramebufferBinder binder(decoder_, fbo);
      api()->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                         Target(), id(), 0);
      api()->glClearColorFn(0, 0, 0, decoder_->BackBufferAlphaClearColor());
      decoder_->state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      decoder_->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
      decoder_->ClearDeviceWindowRectangles();
      api()->glClearFn(GL_COLOR_BUFFER_BIT);
      decoder_->RestoreClearState();
    }
    api()->glDeleteFramebuffersEXTFn(1, &fbo);
  }
  return true;
}
