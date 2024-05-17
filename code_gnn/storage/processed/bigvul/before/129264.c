void GLES2DecoderImpl::DoCopyTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height) {
  DCHECK(!ShouldDeferReads());
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopyTexSubImage2D", "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  GLenum type = 0;
  GLenum format = 0;
  if (!texture->GetLevelType(target, level, &type, &format) ||
      !texture->ValidForTexture(
          target, level, xoffset, yoffset, width, height, type)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glCopyTexSubImage2D", "bad dimensions.");
    return;
  }
  if (async_pixel_transfer_manager_->AsyncTransferIsInProgress(texture_ref)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopyTexSubImage2D", "async upload pending for texture");
    return;
  }

  GLenum read_format = GetBoundReadFrameBufferInternalFormat();
  uint32 channels_exist = GLES2Util::GetChannelsForFormat(read_format);
  uint32 channels_needed = GLES2Util::GetChannelsForFormat(format);

  if (!channels_needed ||
      (channels_needed & channels_exist) != channels_needed) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glCopyTexSubImage2D", "incompatible format");
    return;
  }

  if ((channels_needed & (GLES2Util::kDepth | GLES2Util::kStencil)) != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCopySubImage2D", "can not be used with depth or stencil textures");
    return;
  }

  if (!CheckBoundFramebuffersValid("glCopyTexSubImage2D")) {
    return;
  }

  ScopedResolvedFrameBufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFrameBufferSize();
  GLint copyX = 0;
  GLint copyY = 0;
  GLint copyWidth = 0;
  GLint copyHeight = 0;
  Clip(x, width, size.width(), &copyX, &copyWidth);
  Clip(y, height, size.height(), &copyY, &copyHeight);

  if (!texture_manager()->ClearTextureLevel(this, texture_ref, target, level)) {
    LOCAL_SET_GL_ERROR(
        GL_OUT_OF_MEMORY, "glCopyTexSubImage2D", "dimensions too big");
    return;
  }

  if (copyX != x ||
      copyY != y ||
      copyWidth != width ||
      copyHeight != height) {
    uint32 pixels_size = 0;
    if (!GLES2Util::ComputeImageDataSizes(
        width, height, format, type, state_.unpack_alignment, &pixels_size,
        NULL, NULL)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glCopyTexSubImage2D", "dimensions too large");
      return;
    }
    scoped_ptr<char[]> zero(new char[pixels_size]);
    memset(zero.get(), 0, pixels_size);
    ScopedModifyPixels modify(texture_ref);
    glTexSubImage2D(
        target, level, xoffset, yoffset, width, height,
        format, type, zero.get());
  }

  if (copyHeight > 0 && copyWidth > 0) {
    GLint dx = copyX - x;
    GLint dy = copyY - y;
    GLint destX = xoffset + dx;
    GLint destY = yoffset + dy;
    ScopedModifyPixels modify(texture_ref);
    glCopyTexSubImage2D(target, level,
                        destX, destY, copyX, copyY,
                        copyWidth, copyHeight);
  }
}
