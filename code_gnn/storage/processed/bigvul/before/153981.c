void GLES2DecoderImpl::DoCopyTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height) {
  const char* func_name = "glCopyTexSubImage2D";
  DCHECK(!ShouldDeferReads());
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, func_name, "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  GLenum type = 0;
  GLenum internal_format = 0;
  if (!texture->GetLevelType(target, level, &type, &internal_format) ||
      !texture->ValidForTexture(
          target, level, xoffset, yoffset, 0, width, height, 1)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "bad dimensions.");
    return;
  }

  if (!CheckBoundReadFramebufferValid(func_name,
                                      GL_INVALID_FRAMEBUFFER_OPERATION)) {
    return;
  }

  GLenum read_format = GetBoundReadFramebufferInternalFormat();
  GLenum read_type = GetBoundReadFramebufferTextureType();
  if (!ValidateCopyTexFormat(func_name, internal_format,
                             read_format, read_type)) {
    return;
  }

  if (FormsTextureCopyingFeedbackLoop(texture_ref, level, 0)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        func_name, "source and destination textures are the same");
    return;
  }

  ScopedResolvedFramebufferBinder binder(this, false, true);
  gfx::Size size = GetBoundReadFramebufferSize();

  gfx::Rect src(x, y, width, height);
  const gfx::Rect dst(0, 0, size.width(), size.height());
  src.Intersect(dst);

  if (src.IsEmpty())
    return;

  GLint dx = src.x() - x;
  GLint dy = src.y() - y;
  GLint destX = xoffset + dx;
  GLint destY = yoffset + dy;
  GLsizei level_width = 0;
  GLsizei level_height = 0;
  GLsizei level_depth = 0;
  bool have_level = texture->GetLevelSize(
      target, level, &level_width, &level_height, &level_depth);
  DCHECK(have_level);
  if (destX == 0 && destY == 0 &&
      src.width() == level_width && src.height() == level_height) {
    texture_manager()->SetLevelCleared(texture_ref, target, level, true);
  } else {
    gfx::Rect cleared_rect;
    if (TextureManager::CombineAdjacentRects(
            texture->GetLevelClearedRect(target, level),
            gfx::Rect(destX, destY, src.width(), src.height()),
            &cleared_rect)) {
      DCHECK_GE(cleared_rect.size().GetArea(),
                texture->GetLevelClearedRect(target, level).size().GetArea());
      texture_manager()->SetLevelClearedRect(texture_ref, target, level,
                                             cleared_rect);
    } else {
      if (!texture_manager()->ClearTextureLevel(this, texture_ref, target,
                                                level)) {
        LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, func_name, "dimensions too big");
        return;
      }
    }
  }

  if (CopyTexImageResourceManager::CopyTexImageRequiresBlit(
          feature_info_.get(), internal_format)) {
    if (!InitializeCopyTexImageBlitter("glCopyTexSubImage2D")) {
      return;
    }
    copy_tex_image_blit_->DoCopyTexSubImageToLUMACompatibilityTexture(
        this, texture->service_id(), texture->target(), target,
        internal_format, type, level, destX, destY, 0,
        src.x(), src.y(), src.width(), src.height(),
        GetBoundReadFramebufferServiceId(),
        GetBoundReadFramebufferInternalFormat());
  } else {
    api()->glCopyTexSubImage2DFn(target, level, destX, destY, src.x(), src.y(),
                                 src.width(), src.height());
  }

  ExitCommandProcessingEarly();
}
