error::Error GLES2DecoderImpl::DoTexImage2D(
  GLenum target,
  GLint level,
  GLenum internal_format,
  GLsizei width,
  GLsizei height,
  GLint border,
  GLenum format,
  GLenum type,
  const void* pixels,
  uint32 pixels_size) {
  if (!validators_->texture_target.IsValid(target)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: target GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->texture_format.IsValid(internal_format)) {
    SetGLError(GL_INVALID_ENUM,
               "glTexImage2D: internal_format GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->texture_format.IsValid(format)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: format GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (!validators_->pixel_type.IsValid(type)) {
    SetGLError(GL_INVALID_ENUM, "glTexImage2D: type GL_INVALID_ENUM");
    return error::kNoError;
  }
  if (format != internal_format) {
    SetGLError(GL_INVALID_OPERATION, "glTexImage2D: format != internalFormat");
    return error::kNoError;
  }
  if (!texture_manager()->ValidForTarget(
        feature_info_, target, level, width, height, 1) ||
      border != 0) {
    SetGLError(GL_INVALID_VALUE, "glTexImage2D: dimensions out of range");
    return error::kNoError;
  }
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_OPERATION,
               "glTexImage2D: unknown texture for target");
    return error::kNoError;
  }

  GLsizei tex_width = 0;
  GLsizei tex_height = 0;
  GLenum tex_type = 0;
  GLenum tex_format = 0;
  bool level_is_same =
      info->GetLevelSize(target, level, &tex_width, &tex_height) &&
      info->GetLevelType(target, level, &tex_type, &tex_format) &&
      width == tex_width && height == tex_height &&
      type == tex_type && format == tex_format;

  if (level_is_same && !pixels) {
    tex_image_2d_failed_ = false;
    return error::kNoError;
  }

  scoped_array<int8> zero;
  if (!pixels) {
    zero.reset(new int8[pixels_size]);
    memset(zero.get(), 0, pixels_size);
    pixels = zero.get();
  }

  if (info->IsAttachedToFramebuffer()) {
    state_dirty_ = true;
  }

  if (!teximage2d_faster_than_texsubimage2d_ && level_is_same) {
    glTexSubImage2D(target, level, 0, 0, width, height, format, type, pixels);
    tex_image_2d_failed_ = false;
    return error::kNoError;
  }

  CopyRealGLErrorsToWrapper();
  WrappedTexImage2D(
      target, level, internal_format, width, height, border, format, type,
      pixels);
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(feature_info_, info,
        target, level, internal_format, width, height, 1, border, format, type);
    tex_image_2d_failed_ = false;
  }
  return error::kNoError;
}
