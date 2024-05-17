void GLES2DecoderImpl::DoCompressedCopyTextureCHROMIUM(GLuint source_id,
                                                       GLuint dest_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCompressedCopyTextureCHROMIUM");
  static const char kFunctionName[] = "glCompressedCopyTextureCHROMIUM";
  TextureRef* source_texture_ref = GetTexture(source_id);
  TextureRef* dest_texture_ref = GetTexture(dest_id);

  if (!source_texture_ref || !dest_texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "unknown texture ids");
    return;
  }

  if (!ValidateCompressedCopyTextureCHROMIUM(kFunctionName, source_texture_ref,
                                             dest_texture_ref)) {
    return;
  }

  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  int source_width = 0;
  int source_height = 0;
  gl::GLImage* image =
      source_texture->GetLevelImage(source_texture->target(), 0);
  if (image) {
    gfx::Size size = image->GetSize();
    source_width = size.width();
    source_height = size.height();
    if (source_width <= 0 || source_height <= 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "invalid image size");
      return;
    }
  } else {
    if (!source_texture->GetLevelSize(source_texture->target(), 0,
                                      &source_width, &source_height, nullptr)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                         "source texture has no level 0");
      return;
    }

    if (!texture_manager()->ValidForTarget(source_texture->target(), 0,
                                           source_width, source_height, 1)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "Bad dimensions");
      return;
    }
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(
      source_texture->target(), 0, &source_type, &source_internal_format);

  if (dest_texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "texture is immutable");
    return;
  }

  if (!InitializeCopyTextureCHROMIUM(kFunctionName))
    return;

  if (!texture_manager()->ClearTextureLevel(this, source_texture_ref,
                                            source_texture->target(), 0)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, kFunctionName, "dimensions too big");
    return;
  }

  ScopedTextureBinder binder(
      &state_, dest_texture->service_id(), GL_TEXTURE_2D);

  if (image) {
    GLenum dest_type = 0;
    GLenum dest_internal_format = 0;
    int dest_width = 0;
    int dest_height = 0;
    bool dest_level_defined = dest_texture->GetLevelSize(
        dest_texture->target(), 0, &dest_width, &dest_height, nullptr);

    if (dest_level_defined) {
      dest_texture->GetLevelType(dest_texture->target(), 0, &dest_type,
                                 &dest_internal_format);
    }

    if (!dest_level_defined || dest_width != source_width ||
        dest_height != source_height ||
        dest_internal_format != source_internal_format) {
      GLsizei source_size = 0;

      bool did_get_size = GetCompressedTexSizeInBytes(
          kFunctionName, source_width, source_height, 1, source_internal_format,
          &source_size, state_.GetErrorState());
      DCHECK(did_get_size);

      LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(kFunctionName);
      api()->glCompressedTexImage2DFn(GL_TEXTURE_2D, 0, source_internal_format,
                                      source_width, source_height, 0,
                                      source_size, nullptr);
      GLenum error = LOCAL_PEEK_GL_ERROR(kFunctionName);
      if (error != GL_NO_ERROR) {
        RestoreCurrentTextureBindings(&state_, dest_texture->target(),
                                      state_.active_texture_unit);
        return;
      }

      texture_manager()->SetLevelInfo(
          dest_texture_ref, dest_texture->target(), 0, source_internal_format,
          source_width, source_height, 1, 0, source_internal_format,
          source_type, gfx::Rect(source_width, source_height));
    } else {
      texture_manager()->SetLevelCleared(
          dest_texture_ref, dest_texture->target(), 0, true);
    }

    if (image->CopyTexImage(dest_texture->target()))
      return;
  }

  TRACE_EVENT0(
      "gpu",
      "GLES2DecoderImpl::DoCompressedCopyTextureCHROMIUM, fallback");

  DoBindOrCopyTexImageIfNeeded(source_texture, source_texture->target(), 0);

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(kFunctionName);
  {
    ScopedPixelUnpackState reset_restore(&state_);
    api()->glTexImage2DFn(dest_texture->target(), 0, GL_RGBA, source_width,
                          source_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  }
  GLenum error = LOCAL_PEEK_GL_ERROR(kFunctionName);
  if (error != GL_NO_ERROR) {
    RestoreCurrentTextureBindings(&state_, dest_texture->target(),
                                  state_.active_texture_unit);
    return;
  }

  texture_manager()->SetLevelInfo(
      dest_texture_ref, dest_texture->target(), 0, GL_RGBA, source_width,
      source_height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      gfx::Rect(source_width, source_height));

  copy_texture_chromium_->DoCopyTexture(
      this, source_texture->target(), source_texture->service_id(), 0,
      source_internal_format, dest_texture->target(),
      dest_texture->service_id(), 0, GL_RGBA, source_width, source_height,
      false, false, false, false, CopyTextureMethod::DIRECT_DRAW,
      copy_tex_image_blit_.get());
}
