void GLES2DecoderImpl::DoTexStorage2DImageCHROMIUM(GLenum target,
                                                   GLenum internal_format,
                                                   GLenum buffer_usage,
                                                   GLsizei width,
                                                   GLsizei height) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoTexStorage2DImageCHROMIUM", "width",
               width, "height", height);

  ScopedGLErrorSuppressor suppressor(
      "GLES2CmdDecoder::DoTexStorage2DImageCHROMIUM", error_state_.get());

  if (!texture_manager()->ValidForTarget(target, 0, width, height, 1)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glTexStorage2DImageCHROMIUM",
                       "dimensions out of range");
    return;
  }

  TextureRef* texture_ref =
      texture_manager()->GetTextureInfoForTarget(&state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glTexStorage2DImageCHROMIUM",
                       "unknown texture for target");
    return;
  }

  Texture* texture = texture_ref->texture();
  if (texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glTexStorage2DImageCHROMIUM",
                       "texture is immutable");
    return;
  }

  gfx::BufferFormat buffer_format;
  switch (internal_format) {
    case GL_RGBA8_OES:
      buffer_format = gfx::BufferFormat::RGBA_8888;
      break;
    case GL_BGRA8_EXT:
      buffer_format = gfx::BufferFormat::BGRA_8888;
      break;
    case GL_RGBA16F_EXT:
      buffer_format = gfx::BufferFormat::RGBA_F16;
      break;
    case GL_R8_EXT:
      buffer_format = gfx::BufferFormat::R_8;
      break;
    default:
      LOCAL_SET_GL_ERROR(GL_INVALID_ENUM, "glTexStorage2DImageCHROMIUM",
                         "Invalid buffer format");
      return;
  }

  DCHECK_EQ(buffer_usage, static_cast<GLenum>(GL_SCANOUT_CHROMIUM));

  if (!GetContextGroup()->image_factory()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glTexStorage2DImageCHROMIUM",
                       "Cannot create GL image");
    return;
  }

  bool is_cleared = false;
  scoped_refptr<gl::GLImage> image =
      GetContextGroup()->image_factory()->CreateAnonymousImage(
          gfx::Size(width, height), buffer_format, gfx::BufferUsage::SCANOUT,
          &is_cleared);
  if (!image || !image->BindTexImage(target)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glTexStorage2DImageCHROMIUM",
                       "Failed to create or bind GL Image");
    return;
  }

  gfx::Rect cleared_rect;
  if (is_cleared)
    cleared_rect = gfx::Rect(width, height);

  texture_manager()->SetLevelInfo(
      texture_ref, target, 0, image->GetInternalFormat(), width, height, 1, 0,
      TextureManager::ExtractFormatFromStorageFormat(
          image->GetInternalFormat()),
      GL_UNSIGNED_BYTE, cleared_rect);
  texture_manager()->SetLevelImage(texture_ref, target, 0, image.get(),
                                   Texture::BOUND);

  if (texture->IsAttachedToFramebuffer())
    framebuffer_state_.clear_state_dirty = true;

  texture->SetImmutable(true);
}
