void GLES2DecoderImpl::DoTexStorage2DEXT(
    GLenum target,
    GLint levels,
    GLenum internal_format,
    GLsizei width,
    GLsizei height) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoTexStorage2DEXT",
      "width", width, "height", height);
  if (!texture_manager()->ValidForTarget(target, 0, width, height, 1) ||
      TextureManager::ComputeMipMapCount(target, width, height, 1) < levels) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glTexStorage2DEXT", "dimensions out of range");
    return;
  }
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glTexStorage2DEXT", "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  if (texture->IsAttachedToFramebuffer()) {
    framebuffer_state_.clear_state_dirty = true;
  }
  if (texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glTexStorage2DEXT", "texture is immutable");
    return;
  }

  GLenum format = ExtractFormatFromStorageFormat(internal_format);
  GLenum type = ExtractTypeFromStorageFormat(internal_format);

  {
    GLsizei level_width = width;
    GLsizei level_height = height;
    uint32 estimated_size = 0;
    for (int ii = 0; ii < levels; ++ii) {
      uint32 level_size = 0;
      if (!GLES2Util::ComputeImageDataSizes(
          level_width, level_height, format, type, state_.unpack_alignment,
          &estimated_size, NULL, NULL) ||
          !SafeAddUint32(estimated_size, level_size, &estimated_size)) {
        LOCAL_SET_GL_ERROR(
            GL_OUT_OF_MEMORY, "glTexStorage2DEXT", "dimensions too large");
        return;
      }
      level_width = std::max(1, level_width >> 1);
      level_height = std::max(1, level_height >> 1);
    }
    if (!EnsureGPUMemoryAvailable(estimated_size)) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, "glTexStorage2DEXT", "out of memory");
      return;
    }
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glTexStorage2DEXT");
  glTexStorage2DEXT(target, levels, internal_format, width, height);
  GLenum error = LOCAL_PEEK_GL_ERROR("glTexStorage2DEXT");
  if (error == GL_NO_ERROR) {
    GLsizei level_width = width;
    GLsizei level_height = height;
    for (int ii = 0; ii < levels; ++ii) {
      texture_manager()->SetLevelInfo(
          texture_ref, target, ii, format,
          level_width, level_height, 1, 0, format, type, false);
      level_width = std::max(1, level_width >> 1);
      level_height = std::max(1, level_height >> 1);
    }
    texture->SetImmutable(true);
  }
}
