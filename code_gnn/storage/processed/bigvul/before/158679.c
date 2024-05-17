void GLES2DecoderImpl::DoCopyTextureCHROMIUM(
    GLuint source_id,
    GLint source_level,
    GLenum dest_target,
    GLuint dest_id,
    GLint dest_level,
    GLenum internal_format,
    GLenum dest_type,
    GLboolean unpack_flip_y,
    GLboolean unpack_premultiply_alpha,
    GLboolean unpack_unmultiply_alpha) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCopyTextureCHROMIUM");
  static const char kFunctionName[] = "glCopyTextureCHROMIUM";

  TextureRef* source_texture_ref = GetTexture(source_id);
  TextureRef* dest_texture_ref = GetTexture(dest_id);

  if (!ValidateCopyTextureCHROMIUMTextures(
          kFunctionName, dest_target, source_texture_ref, dest_texture_ref)) {
    return;
  }

  if (source_level < 0 || dest_level < 0 ||
      (feature_info_->IsWebGL1OrES2Context() && source_level > 0)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "source_level or dest_level out of range");
    return;
  }

  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  GLenum source_target = source_texture->target();
  GLenum dest_binding_target = dest_texture->target();

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(source_target, source_level, &source_type,
                               &source_internal_format);
  GLenum format =
      TextureManager::ExtractFormatFromStorageFormat(internal_format);
  if (!texture_manager()->ValidateTextureParameters(
          GetErrorState(), kFunctionName, true, format, dest_type,
          internal_format, dest_level)) {
    return;
  }

  std::string output_error_msg;
  if (!ValidateCopyTextureCHROMIUMInternalFormats(
          GetFeatureInfo(), source_internal_format, internal_format,
          &output_error_msg)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       output_error_msg.c_str());
    return;
  }

  if (feature_info_->feature_flags().desktop_srgb_support) {
    bool enable_framebuffer_srgb =
        GLES2Util::GetColorEncodingFromInternalFormat(source_internal_format) ==
            GL_SRGB ||
        GLES2Util::GetColorEncodingFromInternalFormat(internal_format) ==
            GL_SRGB;
    state_.EnableDisableFramebufferSRGB(enable_framebuffer_srgb);
  }

  int source_width = 0;
  int source_height = 0;
  gl::GLImage* image =
      source_texture->GetLevelImage(source_target, source_level);
  if (image) {
    gfx::Size size = image->GetSize();
    source_width = size.width();
    source_height = size.height();
    if (source_width <= 0 || source_height <= 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "invalid image size");
      return;
    }
  } else {
    if (!source_texture->GetLevelSize(source_target, source_level,
                                      &source_width, &source_height, nullptr)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                         "source texture has no data for level");
      return;
    }

    if (!texture_manager()->ValidForTarget(source_target, source_level,
                                           source_width, source_height, 1)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "Bad dimensions");
      return;
    }
  }

  if (dest_texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "texture is immutable");
    return;
  }

  if (!texture_manager()->ClearTextureLevel(this, source_texture_ref,
                                            source_target, source_level)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, kFunctionName, "dimensions too big");
    return;
  }

  if (!InitializeCopyTextureCHROMIUM(kFunctionName))
    return;

  GLenum dest_type_previous = dest_type;
  GLenum dest_internal_format = internal_format;
  int dest_width = 0;
  int dest_height = 0;
  bool dest_level_defined = dest_texture->GetLevelSize(
      dest_target, dest_level, &dest_width, &dest_height, nullptr);

  if (dest_level_defined) {
    dest_texture->GetLevelType(dest_target, dest_level, &dest_type_previous,
                               &dest_internal_format);
  }

  if (!dest_level_defined || dest_width != source_width ||
      dest_height != source_height ||
      dest_internal_format != internal_format ||
      dest_type_previous != dest_type) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(kFunctionName);
    api()->glBindTextureFn(dest_binding_target, dest_texture->service_id());
    ScopedPixelUnpackState reset_restore(&state_);
    api()->glTexImage2DFn(
        dest_target, dest_level,
        TextureManager::AdjustTexInternalFormat(feature_info_.get(),
                                                internal_format),
        source_width, source_height, 0,
        TextureManager::AdjustTexFormat(feature_info_.get(), format), dest_type,
        nullptr);
    GLenum error = LOCAL_PEEK_GL_ERROR(kFunctionName);
    if (error != GL_NO_ERROR) {
      RestoreCurrentTextureBindings(&state_, dest_binding_target,
                                    state_.active_texture_unit);
      return;
    }

    texture_manager()->SetLevelInfo(dest_texture_ref, dest_target, dest_level,
                                    internal_format, source_width,
                                    source_height, 1, 0, format, dest_type,
                                    gfx::Rect(source_width, source_height));
    dest_texture->ApplyFormatWorkarounds(feature_info_.get());
  } else {
    texture_manager()->SetLevelCleared(dest_texture_ref, dest_target,
                                       dest_level, true);
  }

  bool unpack_premultiply_alpha_change =
      (unpack_premultiply_alpha ^ unpack_unmultiply_alpha) != 0;
  if (image && internal_format == source_internal_format && dest_level == 0 &&
      !unpack_flip_y && !unpack_premultiply_alpha_change) {
    api()->glBindTextureFn(dest_binding_target, dest_texture->service_id());
    if (image->CopyTexImage(dest_target))
      return;
  }

  DoBindOrCopyTexImageIfNeeded(source_texture, source_target, 0);

  if (source_target == GL_TEXTURE_EXTERNAL_OES) {
    if (GLStreamTextureImage* image =
            source_texture->GetLevelStreamTextureImage(GL_TEXTURE_EXTERNAL_OES,
                                                       source_level)) {
      GLfloat transform_matrix[16];
      image->GetTextureMatrix(transform_matrix);
      copy_texture_chromium_->DoCopyTextureWithTransform(
          this, source_target, source_texture->service_id(), source_level,
          source_internal_format, dest_target, dest_texture->service_id(),
          dest_level, internal_format, source_width, source_height,
          unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
          unpack_unmultiply_alpha == GL_TRUE, false  ,
          transform_matrix, copy_tex_image_blit_.get());
      return;
    }
  }

  CopyTextureMethod method = GetCopyTextureCHROMIUMMethod(
      GetFeatureInfo(), source_target, source_level, source_internal_format,
      source_type, dest_binding_target, dest_level, internal_format,
      unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
      unpack_unmultiply_alpha == GL_TRUE, false  );
  copy_texture_chromium_->DoCopyTexture(
      this, source_target, source_texture->service_id(), source_level,
      source_internal_format, dest_target, dest_texture->service_id(),
      dest_level, internal_format, source_width, source_height,
      unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
      unpack_unmultiply_alpha == GL_TRUE, false  , method,
      copy_tex_image_blit_.get());
}
