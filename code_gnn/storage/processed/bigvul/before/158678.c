void GLES2DecoderImpl::DoCopyTexImage2D(
    GLenum target,
    GLint level,
    GLenum internal_format,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLint border) {
  const char* func_name = "glCopyTexImage2D";
  DCHECK(!ShouldDeferReads());
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, func_name, "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  if (texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, func_name, "texture is immutable");
    return;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, 1) ||
      border != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, func_name, "dimensions out of range");
    return;
  }

  if (!CheckBoundReadFramebufferValid(func_name,
                                      GL_INVALID_FRAMEBUFFER_OPERATION)) {
    return;
  }

  GLenum read_format = GetBoundReadFramebufferInternalFormat();
  GLenum read_type = GetBoundReadFramebufferTextureType();
  if (!ValidateCopyTexFormat(func_name, internal_format,
                             read_format, read_type)) {
    return;
  }

  uint32_t pixels_size = 0;
  GLenum format =
      TextureManager::ExtractFormatFromStorageFormat(internal_format);
  GLenum type = TextureManager::ExtractTypeFromStorageFormat(internal_format);
  bool internal_format_unsized = internal_format == format;
  if (internal_format_unsized && feature_info_->IsWebGL2OrES3Context()) {
    DCHECK(type == GL_UNSIGNED_BYTE);
    switch (internal_format) {
      case GL_RGB:
      case GL_RGBA:
      case GL_LUMINANCE_ALPHA:
      case GL_LUMINANCE:
      case GL_ALPHA:
      case GL_BGRA_EXT:
        break;
      default:
        format = GL_NONE;
        break;
    }
  }
  if (!format || !type) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        func_name, "Invalid unsized internal format.");
    return;
  }

  DCHECK(texture_manager()->ValidateTextureParameters(
      GetErrorState(), func_name, true, format, type, internal_format, level));

  if (!GLES2Util::ComputeImageDataSizes(width, height, 1, format, type,
                                        state_.unpack_alignment, &pixels_size,
                                        nullptr, nullptr)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, func_name, "dimensions too large");
    return;
  }

  if (FormsTextureCopyingFeedbackLoop(texture_ref, level, 0)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        func_name, "source and destination textures are the same");
    return;
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(func_name);
  ScopedResolvedFramebufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFramebufferSize();

  if (texture->IsAttachedToFramebuffer()) {
    framebuffer_state_.clear_state_dirty = true;
  }

  bool requires_luma_blit =
      CopyTexImageResourceManager::CopyTexImageRequiresBlit(feature_info_.get(),
                                                            format);
  if (requires_luma_blit &&
      !InitializeCopyTexImageBlitter(func_name)) {
    return;
  }

  bool reset_source_texture_base_level_max_level = false;
  GLint attached_texture_level = -1;
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (framebuffer) {
    const Framebuffer::Attachment* attachment =
        framebuffer->GetReadBufferAttachment();
    if (attachment->IsTexture(texture_ref)) {
      DCHECK(attachment->IsTextureAttachment());
      attached_texture_level = attachment->level();
      DCHECK_GE(attached_texture_level, 0);
      if (attached_texture_level != texture->base_level())
        reset_source_texture_base_level_max_level = true;
    }
  }
  if (reset_source_texture_base_level_max_level) {
    api()->glTexParameteriFn(target, GL_TEXTURE_BASE_LEVEL,
                             attached_texture_level);
    api()->glTexParameteriFn(target, GL_TEXTURE_MAX_LEVEL,
                             attached_texture_level);
  }

  gfx::Rect src(x, y, width, height);
  const gfx::Rect dst(0, 0, size.width(), size.height());
  src.Intersect(dst);

  GLenum final_internal_format = TextureManager::AdjustTexInternalFormat(
      feature_info_.get(), internal_format);
  if (workarounds().force_int_or_srgb_cube_texture_complete &&
      texture->target() == GL_TEXTURE_CUBE_MAP &&
      (GLES2Util::IsIntegerFormat(final_internal_format) ||
       GLES2Util::GetColorEncodingFromInternalFormat(final_internal_format) ==
           GL_SRGB)) {
    TextureManager::DoTexImageArguments args = {
        target,
        level,
        final_internal_format,
        width,
        height,
        1,
        border,
        format,
        type,
        nullptr,
        pixels_size,
        0,
        TextureManager::DoTexImageArguments::kTexImage2D};
    texture_manager()->WorkaroundCopyTexImageCubeMap(
        &texture_state_, &state_, &framebuffer_state_, texture_ref, func_name,
        args);
  }

  if (src.x() != x || src.y() != y ||
      src.width() != width || src.height() != height ||
      final_internal_format == GL_BGRA_EXT) {
    {

      std::unique_ptr<char[]> zero(new char[pixels_size]);
      memset(zero.get(), 0, pixels_size);
      ScopedPixelUnpackState reset_restore(&state_);
      api()->glTexImage2DFn(target, level, final_internal_format, width, height,
                            border, format, type, zero.get());
    }

    if (!src.IsEmpty()) {
      GLint destX = src.x() - x;
      GLint destY = src.y() - y;
      if (requires_luma_blit) {
        copy_tex_image_blit_->DoCopyTexSubImageToLUMACompatibilityTexture(
            this, texture->service_id(), texture->target(), target, format,
            type, level, destX, destY, 0,
            src.x(), src.y(), src.width(), src.height(),
            GetBoundReadFramebufferServiceId(),
            GetBoundReadFramebufferInternalFormat());
      } else {
        api()->glCopyTexSubImage2DFn(target, level, destX, destY, src.x(),
                                     src.y(), src.width(), src.height());
      }
    }
  } else {
    if (workarounds().init_two_cube_map_levels_before_copyteximage &&
        texture->target() == GL_TEXTURE_CUBE_MAP &&
        target != GL_TEXTURE_CUBE_MAP_POSITIVE_X) {
      for (int i = 0; i < 2; ++i) {
        TextureManager::DoTexImageArguments args = {
          target, i, final_internal_format, width, height, 1, border,
          format, type, nullptr, pixels_size, 0,
          TextureManager::DoTexImageArguments::kTexImage2D };
        texture_manager()->WorkaroundCopyTexImageCubeMap(&texture_state_,
            &state_, &framebuffer_state_, texture_ref, func_name, args);
      }
    }

    GLuint source_texture_service_id = 0;
    GLenum source_texture_target = 0;
    uint32_t channels_exist = GLES2Util::GetChannelsForFormat(read_format);
    bool use_workaround = NeedsCopyTextureImageWorkaround(
        final_internal_format, channels_exist, &source_texture_service_id,
        &source_texture_target);
    if (requires_luma_blit) {
      copy_tex_image_blit_->DoCopyTexImage2DToLUMACompatibilityTexture(
          this, texture->service_id(), texture->target(), target, format,
          type, level, internal_format, x, y, width, height,
          GetBoundReadFramebufferServiceId(),
          GetBoundReadFramebufferInternalFormat());
    } else if (use_workaround) {
      GLenum dest_texture_target = target;
      GLenum framebuffer_target = GetReadFramebufferTarget();

      GLenum temp_internal_format = 0;
      if (channels_exist == GLES2Util::kRGBA) {
        temp_internal_format = GL_RGBA;
      } else if (channels_exist == GLES2Util::kRGB) {
        temp_internal_format = GL_RGB;
      } else {
        NOTREACHED();
      }

      if (workarounds().clear_pixel_unpack_buffer_before_copyteximage)
        state_.PushTextureUnpackState();
      GLuint temp_texture;
      {
        api()->glGenTexturesFn(1, &temp_texture);
        ScopedTextureBinder binder(&state_, temp_texture,
                                   source_texture_target);
        api()->glCopyTexImage2DFn(source_texture_target, 0,
                                  temp_internal_format, x, y, width, height,
                                  border);

        api()->glFramebufferTexture2DEXTFn(
            framebuffer_target, GL_COLOR_ATTACHMENT0, source_texture_target,
            temp_texture, 0);
      }

      DCHECK_EQ(static_cast<GLuint>(GL_TEXTURE_2D), dest_texture_target);
      api()->glCopyTexImage2DFn(dest_texture_target, level,
                                final_internal_format, 0, 0, width, height, 0);

      api()->glFramebufferTexture2DEXTFn(
          framebuffer_target, GL_COLOR_ATTACHMENT0, source_texture_target,
          source_texture_service_id, 0);
      if (workarounds().clear_pixel_unpack_buffer_before_copyteximage)
        state_.RestoreUnpackState();

      api()->glDeleteTexturesFn(1, &temp_texture);
    } else {
      if (workarounds().init_one_cube_map_level_before_copyteximage &&
          texture->target() == GL_TEXTURE_CUBE_MAP &&
          target != GL_TEXTURE_CUBE_MAP_POSITIVE_X) {
        TextureManager::DoTexImageArguments args = {
          target, level, final_internal_format, width, height, 1, border,
          format, type, nullptr, pixels_size, 0,
          TextureManager::DoTexImageArguments::kTexImage2D };
        texture_manager()->WorkaroundCopyTexImageCubeMap(&texture_state_,
            &state_, &framebuffer_state_, texture_ref, func_name, args);
      }
      if (workarounds().clear_pixel_unpack_buffer_before_copyteximage)
        state_.PushTextureUnpackState();
      api()->glCopyTexImage2DFn(target, level, final_internal_format, x, y,
                                width, height, border);
      if (workarounds().clear_pixel_unpack_buffer_before_copyteximage)
        state_.RestoreUnpackState();
    }
  }
  if (reset_source_texture_base_level_max_level) {
    api()->glTexParameteriFn(target, GL_TEXTURE_BASE_LEVEL,
                             texture->base_level());
    api()->glTexParameteriFn(target, GL_TEXTURE_MAX_LEVEL,
                             texture->max_level());
  }
  GLenum error = LOCAL_PEEK_GL_ERROR(func_name);
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(texture_ref, target, level, internal_format,
                                    width, height, 1, border, format,
                                    type, gfx::Rect(width, height));
    texture->ApplyFormatWorkarounds(feature_info_.get());
  }

  ExitCommandProcessingEarly();
}
