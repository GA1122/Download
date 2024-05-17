void GLES2DecoderImpl::DoCopyTexImage2D(
    GLenum target,
    GLint level,
    GLenum internal_format,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLint border) {
  DCHECK(!ShouldDeferReads());
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopyTexImage2D", "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  if (texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glCopyTexImage2D", "texture is immutable");
    return;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, 1) ||
      border != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glCopyTexImage2D", "dimensions out of range");
    return;
  }
  if (!texture_manager()->ValidateFormatAndTypeCombination(
      state_.GetErrorState(), "glCopyTexImage2D", internal_format,
      GL_UNSIGNED_BYTE)) {
    return;
  }

  GLenum read_format = GetBoundReadFrameBufferInternalFormat();
  uint32 channels_exist = GLES2Util::GetChannelsForFormat(read_format);
  uint32 channels_needed = GLES2Util::GetChannelsForFormat(internal_format);

  if ((channels_needed & channels_exist) != channels_needed) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glCopyTexImage2D", "incompatible format");
    return;
  }

  if ((channels_needed & (GLES2Util::kDepth | GLES2Util::kStencil)) != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopyTexImage2D", "can not be used with depth or stencil textures");
    return;
  }

  uint32 estimated_size = 0;
  if (!GLES2Util::ComputeImageDataSizes(
      width, height, 1, internal_format, GL_UNSIGNED_BYTE,
      state_.unpack_alignment, &estimated_size, NULL, NULL)) {
    LOCAL_SET_GL_ERROR(
        GL_OUT_OF_MEMORY, "glCopyTexImage2D", "dimensions too large");
    return;
  }

  if (!EnsureGPUMemoryAvailable(estimated_size)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, "glCopyTexImage2D", "out of memory");
    return;
  }

  if (!CheckBoundReadFramebufferColorAttachment("glCopyTexImage2D")) {
    return;
  }

  if (FormsTextureCopyingFeedbackLoop(texture_ref, level)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopyTexImage2D", "source and destination textures are the same");
    return;
  }

  if (!CheckBoundFramebuffersValid("glCopyTexImage2D")) {
    return;
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glCopyTexImage2D");
  ScopedResolvedFrameBufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFrameBufferSize();

  if (texture->IsAttachedToFramebuffer()) {
    framebuffer_state_.clear_state_dirty = true;
  }

  GLint copyX = 0;
  GLint copyY = 0;
  GLint copyWidth = 0;
  GLint copyHeight = 0;
  Clip(x, width, size.width(), &copyX, &copyWidth);
  Clip(y, height, size.height(), &copyY, &copyHeight);

  if (copyX != x ||
      copyY != y ||
      copyWidth != width ||
      copyHeight != height) {
    if (!ClearLevel(texture, target, level, internal_format, internal_format,
                    GL_UNSIGNED_BYTE, width, height, texture->IsImmutable())) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, "glCopyTexImage2D", "dimensions too big");
      return;
    }
    if (copyHeight > 0 && copyWidth > 0) {
      GLint dx = copyX - x;
      GLint dy = copyY - y;
      GLint destX = dx;
      GLint destY = dy;
      ScopedModifyPixels modify(texture_ref);
      glCopyTexSubImage2D(target, level,
                          destX, destY, copyX, copyY,
                          copyWidth, copyHeight);
    }
  } else {
    ScopedModifyPixels modify(texture_ref);
    glCopyTexImage2D(target, level, internal_format,
                     copyX, copyY, copyWidth, copyHeight, border);
  }
  GLenum error = LOCAL_PEEK_GL_ERROR("glCopyTexImage2D");
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(
        texture_ref, target, level, internal_format, width, height, 1,
        border, internal_format, GL_UNSIGNED_BYTE, true);
  }

  ExitCommandProcessingEarly();
}
