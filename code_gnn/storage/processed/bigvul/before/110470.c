void GLES2DecoderImpl::DoCopyTextureCHROMIUM(
    GLenum target, GLuint source_id, GLuint dest_id, GLint level,
    GLenum internal_format) {
  TextureManager::TextureInfo* dest_info = GetTextureInfo(dest_id);
  TextureManager::TextureInfo* source_info = GetTextureInfo(source_id);

  if (!source_info || !dest_info) {
    SetGLError(GL_INVALID_VALUE, "glCopyTextureCHROMIUM", "unknown texture id");
    return;
  }

  if (GL_TEXTURE_2D != target) {
    SetGLError(GL_INVALID_VALUE,
               "glCopyTextureCHROMIUM", "invalid texture target");
    return;
  }

  if (dest_info->target() != GL_TEXTURE_2D ||
      source_info->target() != GL_TEXTURE_2D) {
    SetGLError(GL_INVALID_VALUE,
               "glCopyTextureCHROMIUM", "invalid texture target binding");
    return;
  }

  int source_width, source_height, dest_width, dest_height;
  if (!source_info->GetLevelSize(GL_TEXTURE_2D, 0, &source_width,
                                 &source_height)) {
    SetGLError(GL_INVALID_VALUE,
               "glCopyTextureChromium", "source texture has no level 0");
    return;
  }

  if (!texture_manager()->ValidForTarget(GL_TEXTURE_2D, level, source_width,
                                         source_height, 1)) {
    SetGLError(GL_INVALID_VALUE,
             "glCopyTextureCHROMIUM", "Bad dimensions");
    return;
  }

  if (!copy_texture_CHROMIUM_.get()) {
    CopyRealGLErrorsToWrapper();
    copy_texture_CHROMIUM_.reset(new CopyTextureCHROMIUMResourceManager());
    copy_texture_CHROMIUM_->Initialize();
    RestoreCurrentFramebufferBindings();
    if (PeekGLError() != GL_NO_ERROR)
      return;
  }

  GLenum dest_type;
  GLenum dest_internal_format;
  bool dest_level_defined = dest_info->GetLevelSize(GL_TEXTURE_2D, level,
                                                    &dest_width,
                                                    &dest_height);

  if (dest_level_defined) {
    dest_info->GetLevelType(GL_TEXTURE_2D, level, &dest_type,
                            &dest_internal_format);
  } else {
    GLenum source_internal_format;
    source_info->GetLevelType(GL_TEXTURE_2D, 0, &dest_type,
                              &source_internal_format);
  }

  if (!dest_level_defined || dest_width != source_width ||
      dest_height != source_height ||
      dest_internal_format != internal_format) {
    CopyRealGLErrorsToWrapper();
    glBindTexture(GL_TEXTURE_2D, dest_info->service_id());
    WrappedTexImage2D(
        GL_TEXTURE_2D, level, internal_format, source_width, source_height,
        0, internal_format, dest_type, NULL);
    GLenum error = PeekGLError();
    if (error != GL_NO_ERROR) {
      RestoreCurrentTexture2DBindings();
      return;
    }

    texture_manager()->SetLevelInfo(
        dest_info, GL_TEXTURE_2D, level, internal_format, source_width,
        source_height, 1, 0, internal_format, dest_type, true);
  } else {
    texture_manager()->SetLevelCleared(dest_info, GL_TEXTURE_2D, level);
  }

  state_dirty_ = true;
  glViewport(0, 0, source_width, source_height);
  copy_texture_CHROMIUM_->DoCopyTexture(target, source_info->service_id(),
                                        dest_info->service_id(), level,
                                        unpack_flip_y_,
                                        unpack_premultiply_alpha_,
                                        unpack_unpremultiply_alpha_);
  glViewport(viewport_x_, viewport_y_, viewport_width_, viewport_height_);

  if (current_program_)
    glUseProgram(current_program_->service_id());
  else
    glUseProgram(0);

  RestoreCurrentFramebufferBindings();
  RestoreCurrentTexture2DBindings();
  RestoreStateForAttrib(
      CopyTextureCHROMIUMResourceManager::kVertexPositionAttrib);
  RestoreStateForAttrib(
      CopyTextureCHROMIUMResourceManager::kVertexTextureAttrib);

  ApplyDirtyState();
}
