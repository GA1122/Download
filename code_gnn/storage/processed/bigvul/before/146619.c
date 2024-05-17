RefPtr<DrawingBuffer::ColorBuffer> DrawingBuffer::CreateColorBuffer(
    const IntSize& size) {
  DCHECK(state_restorer_);
  state_restorer_->SetFramebufferBindingDirty();
  state_restorer_->SetTextureBindingDirty();

  ColorBufferParameters parameters;
  GLuint image_id = 0;
  std::unique_ptr<gfx::GpuMemoryBuffer> gpu_memory_buffer;
  gpu::GpuMemoryBufferManager* gpu_memory_buffer_manager =
      Platform::Current()->GetGpuMemoryBufferManager();
  if (ShouldUseChromiumImage() && gpu_memory_buffer_manager) {
    parameters = GpuMemoryBufferColorBufferParameters();
    gfx::BufferFormat buffer_format;
    GLenum gl_format = GL_NONE;
    if (parameters.allocate_alpha_channel) {
      buffer_format = gfx::BufferFormat::RGBA_8888;
      gl_format = GL_RGBA;
    } else {
      buffer_format = gfx::BufferFormat::BGRX_8888;
      gl_format = GL_RGB;
    }
    gpu_memory_buffer = gpu_memory_buffer_manager->CreateGpuMemoryBuffer(
        gfx::Size(size), buffer_format, gfx::BufferUsage::SCANOUT,
        gpu::kNullSurfaceHandle);
    if (gpu_memory_buffer) {
      if (RuntimeEnabledFeatures::ColorCorrectRenderingEnabled())
        gpu_memory_buffer->SetColorSpaceForScanout(color_space_);
      image_id =
          gl_->CreateImageCHROMIUM(gpu_memory_buffer->AsClientBuffer(),
                                   size.Width(), size.Height(), gl_format);
      if (!image_id)
        gpu_memory_buffer.reset();
    }
  } else {
    parameters = TextureColorBufferParameters();
  }

  GLuint texture_id = 0;
  {
    gl_->GenTextures(1, &texture_id);
    gl_->BindTexture(parameters.target, texture_id);
    gl_->TexParameteri(parameters.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl_->TexParameteri(parameters.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_->TexParameteri(parameters.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl_->TexParameteri(parameters.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  if (image_id) {
    gl_->BindTexImage2DCHROMIUM(parameters.target, image_id);
  } else {
    if (storage_texture_supported_) {
      GLenum internal_storage_format =
          parameters.allocate_alpha_channel ? GL_RGBA8 : GL_RGB8;
      gl_->TexStorage2DEXT(GL_TEXTURE_2D, 1, internal_storage_format,
                           size.Width(), size.Height());
    } else {
      GLenum gl_format = parameters.allocate_alpha_channel ? GL_RGBA : GL_RGB;
      gl_->TexImage2D(parameters.target, 0, gl_format, size.Width(),
                      size.Height(), 0, gl_format, GL_UNSIGNED_BYTE, 0);
    }
  }

  if (image_id && !want_alpha_channel_ &&
      ContextProvider()->GetCapabilities().chromium_image_rgb_emulation) {
    GLuint fbo = 0;

    state_restorer_->SetClearStateDirty();
    gl_->GenFramebuffers(1, &fbo);
    gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo);
    gl_->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              parameters.target, texture_id, 0);
    gl_->ClearColor(0, 0, 0, 1);
    gl_->ColorMask(false, false, false, true);
    gl_->Clear(GL_COLOR_BUFFER_BIT);
    gl_->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              parameters.target, 0, 0);
    gl_->DeleteFramebuffers(1, &fbo);
  }

  return AdoptRef(new ColorBuffer(this, parameters, size, texture_id, image_id,
                                  std::move(gpu_memory_buffer)));
}
