void GLES2DecoderImpl::DoCopyTextureCHROMIUM(GLenum target,
                                             GLuint source_id,
                                             GLuint dest_id,
                                             GLenum internal_format,
                                             GLenum dest_type) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCopyTextureCHROMIUM");

  TextureRef* source_texture_ref = GetTexture(source_id);
  TextureRef* dest_texture_ref = GetTexture(dest_id);
  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  int source_width = 0;
  int source_height = 0;
  gfx::GLImage* image =
      source_texture->GetLevelImage(source_texture->target(), 0);
  if (image) {
    gfx::Size size = image->GetSize();
    source_width = size.width();
    source_height = size.height();
    if (source_width <= 0 || source_height <= 0) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE,
          "glCopyTextureChromium", "invalid image size");
      return;
    }
  } else {
    if (!source_texture->GetLevelSize(
             source_texture->target(), 0, &source_width, &source_height)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE,
                         "glCopyTextureChromium",
                         "source texture has no level 0");
      return;
    }

    if (!texture_manager()->ValidForTarget(source_texture->target(), 0,
                                           source_width, source_height, 1)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glCopyTextureCHROMIUM", "Bad dimensions");
      return;
    }
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(
      source_texture->target(), 0, &source_type, &source_internal_format);

  if (dest_texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glCopyTextureCHROMIUM",
                       "texture is immutable");
    return;
  }

  if (!ValidateCopyTextureCHROMIUM("glCopyTextureCHROMIUM", target,
                                   source_texture_ref, dest_texture_ref,
                                   internal_format)) {
    return;
  }

  if (!texture_manager()->ClearTextureLevel(this, source_texture_ref,
                                            source_texture->target(), 0)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, "glCopyTextureCHROMIUM",
                       "dimensions too big");
    return;
  }

  if (!copy_texture_CHROMIUM_.get()) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glCopyTextureCHROMIUM");
    copy_texture_CHROMIUM_.reset(new CopyTextureCHROMIUMResourceManager());
    copy_texture_CHROMIUM_->Initialize(this);
    RestoreCurrentFramebufferBindings();
    if (LOCAL_PEEK_GL_ERROR("glCopyTextureCHROMIUM") != GL_NO_ERROR)
      return;
  }

  GLenum dest_type_previous = dest_type;
  GLenum dest_internal_format = internal_format;
  int dest_width = 0;
  int dest_height = 0;
  bool dest_level_defined =
      dest_texture->GetLevelSize(GL_TEXTURE_2D, 0, &dest_width, &dest_height);

  if (dest_level_defined) {
    dest_texture->GetLevelType(GL_TEXTURE_2D, 0, &dest_type_previous,
                               &dest_internal_format);
  }

  if (!dest_level_defined || dest_width != source_width ||
      dest_height != source_height ||
      dest_internal_format != internal_format ||
      dest_type_previous != dest_type) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glCopyTextureCHROMIUM");
    glBindTexture(GL_TEXTURE_2D, dest_texture->service_id());
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, source_width, source_height,
                 0, internal_format, dest_type, NULL);
    GLenum error = LOCAL_PEEK_GL_ERROR("glCopyTextureCHROMIUM");
    if (error != GL_NO_ERROR) {
      RestoreCurrentTextureBindings(&state_, GL_TEXTURE_2D);
      return;
    }

    texture_manager()->SetLevelInfo(
        dest_texture_ref, GL_TEXTURE_2D, 0, internal_format, source_width,
        source_height, 1, 0, internal_format, dest_type, true);
  } else {
    texture_manager()->SetLevelCleared(dest_texture_ref, GL_TEXTURE_2D, 0,
                                       true);
  }

  ScopedModifyPixels modify(dest_texture_ref);

  bool unpack_premultiply_alpha_change =
      unpack_premultiply_alpha_ ^ unpack_unpremultiply_alpha_;
  if (image && !unpack_flip_y_ && !unpack_premultiply_alpha_change) {
    glBindTexture(GL_TEXTURE_2D, dest_texture->service_id());
    if (image->CopyTexImage(GL_TEXTURE_2D))
      return;
  }

  DoWillUseTexImageIfNeeded(source_texture, source_texture->target());

  if (source_texture->target() == GL_TEXTURE_EXTERNAL_OES) {
    copy_texture_CHROMIUM_->DoCopyTextureWithTransform(
        this, source_texture->target(), source_texture->service_id(),
        dest_texture->service_id(), source_width, source_height, unpack_flip_y_,
        unpack_premultiply_alpha_, unpack_unpremultiply_alpha_,
        kIdentityMatrix);
  } else {
    copy_texture_CHROMIUM_->DoCopyTexture(
        this, source_texture->target(), source_texture->service_id(),
        source_internal_format, dest_texture->service_id(), internal_format,
        source_width, source_height, unpack_flip_y_, unpack_premultiply_alpha_,
        unpack_unpremultiply_alpha_);
  }

  DoDidUseTexImageIfNeeded(source_texture, source_texture->target());
}
