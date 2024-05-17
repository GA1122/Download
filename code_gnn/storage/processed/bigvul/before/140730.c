void GLES2DecoderImpl::DoCopySubTextureCHROMIUM(GLenum target,
                                                GLuint source_id,
                                                GLuint dest_id,
                                                GLint xoffset,
                                                GLint yoffset) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCopySubTextureCHROMIUM");

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
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glCopySubTextureCHROMIUM",
                         "invalid image size");
      return;
    }
  } else {
    if (!source_texture->GetLevelSize(source_texture->target(), 0,
                                      &source_width, &source_height)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glCopySubTextureCHROMIUM",
                         "source texture has no level 0");
      return;
    }

    if (!texture_manager()->ValidForTarget(source_texture->target(), 0,
                                           source_width, source_height, 1)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glCopySubTextureCHROMIUM",
                         "source texture bad dimensions");
      return;
    }
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(source_texture->target(), 0, &source_type,
                               &source_internal_format);
  GLenum dest_type = 0;
  GLenum dest_internal_format = 0;
  bool dest_level_defined = dest_texture->GetLevelType(
      dest_texture->target(), 0, &dest_type, &dest_internal_format);
  if (!dest_level_defined) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glCopySubTextureCHROMIUM",
                       "destination texture is not defined");
    return;
  }
  if (!dest_texture->ValidForTexture(dest_texture->target(), 0, xoffset,
                                     yoffset, source_width, source_height,
                                     dest_type)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glCopySubTextureCHROMIUM",
                       "destination texture bad dimensions.");
    return;
  }

  if (!ValidateCopyTextureCHROMIUM("glCopySubTextureCHROMIUM", target,
                                   source_texture_ref, dest_texture_ref,
                                   dest_internal_format)) {
    return;
  }

  if (!texture_manager()->ClearTextureLevel(this, source_texture_ref,
                                            source_texture->target(), 0)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, "glCopySubTextureCHROMIUM",
                       "source texture dimensions too big");
    return;
  }

  if (!copy_texture_CHROMIUM_.get()) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glCopySubTextureCHROMIUM");
    copy_texture_CHROMIUM_.reset(new CopyTextureCHROMIUMResourceManager());
    copy_texture_CHROMIUM_->Initialize(this);
    RestoreCurrentFramebufferBindings();
    if (LOCAL_PEEK_GL_ERROR("glCopySubTextureCHROMIUM") != GL_NO_ERROR)
      return;
  }

  int dest_width = 0;
  int dest_height = 0;
  bool ok =
      dest_texture->GetLevelSize(GL_TEXTURE_2D, 0, &dest_width, &dest_height);
  DCHECK(ok);
  if (xoffset != 0 || yoffset != 0 || source_width != dest_width ||
      source_height != dest_height) {
    if (!texture_manager()->ClearTextureLevel(this, dest_texture_ref, target,
                                              0)) {
      LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, "glCopySubTextureCHROMIUM",
                         "destination texture dimensions too big");
      return;
    }
  } else {
    texture_manager()->SetLevelCleared(dest_texture_ref, GL_TEXTURE_2D, 0,
                                       true);
  }

  ScopedModifyPixels modify(dest_texture_ref);

  bool unpack_premultiply_alpha_change =
      unpack_premultiply_alpha_ ^ unpack_unpremultiply_alpha_;
  if (image && !unpack_flip_y_ && !unpack_premultiply_alpha_change &&
      !xoffset && !yoffset) {
    glBindTexture(GL_TEXTURE_2D, dest_texture->service_id());
    if (image->CopyTexImage(GL_TEXTURE_2D))
      return;
  }

  DoWillUseTexImageIfNeeded(source_texture, source_texture->target());

  if (source_texture->target() == GL_TEXTURE_EXTERNAL_OES) {
    copy_texture_CHROMIUM_->DoCopySubTextureWithTransform(
        this, source_texture->target(), source_texture->service_id(),
        dest_texture->service_id(), xoffset, yoffset, dest_width, dest_height,
        source_width, source_height, unpack_flip_y_, unpack_premultiply_alpha_,
        unpack_unpremultiply_alpha_, kIdentityMatrix);
  } else {
    copy_texture_CHROMIUM_->DoCopySubTexture(
        this, source_texture->target(), source_texture->service_id(),
        source_internal_format, dest_texture->service_id(),
        dest_internal_format, xoffset, yoffset, dest_width, dest_height,
        source_width, source_height, unpack_flip_y_, unpack_premultiply_alpha_,
        unpack_unpremultiply_alpha_);
  }

  DoDidUseTexImageIfNeeded(source_texture, source_texture->target());
}
