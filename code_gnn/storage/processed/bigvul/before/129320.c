error::Error GLES2DecoderImpl::DoTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    const void * data) {
  error::Error error = error::kNoError;
  if (!ValidateTexSubImage2D(&error, "glTexSubImage2D", target, level,
      xoffset, yoffset, width, height, format, type, data)) {
    return error;
  }
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  Texture* texture = texture_ref->texture();
  GLsizei tex_width = 0;
  GLsizei tex_height = 0;
  bool ok = texture->GetLevelSize(target, level, &tex_width, &tex_height);
  DCHECK(ok);
  if (xoffset != 0 || yoffset != 0 ||
      width != tex_width || height != tex_height) {
    if (!texture_manager()->ClearTextureLevel(this, texture_ref,
                                              target, level)) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, "glTexSubImage2D", "dimensions too big");
      return error::kNoError;
    }
    ScopedTextureUploadTimer timer(&texture_state_);
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height, format, type, data);
    return error::kNoError;
  }

  if (!texture_state_.texsubimage2d_faster_than_teximage2d &&
      !texture->IsImmutable()) {
    ScopedTextureUploadTimer timer(&texture_state_);
    GLenum internal_format;
    GLenum tex_type;
    texture->GetLevelType(target, level, &tex_type, &internal_format);
    glTexImage2D(
        target, level, internal_format, width, height, 0, format, type, data);
  } else {
    ScopedTextureUploadTimer timer(&texture_state_);
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height, format, type, data);
  }
  texture_manager()->SetLevelCleared(texture_ref, target, level, true);
  return error::kNoError;
}
