bool GLES2DecoderImpl::ClearLevel(Texture* texture,
                                  unsigned target,
                                  int level,
                                  unsigned format,
                                  unsigned type,
                                  int xoffset,
                                  int yoffset,
                                  int width,
                                  int height) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::ClearLevel");
  DCHECK(target != GL_TEXTURE_3D && target != GL_TEXTURE_2D_ARRAY &&
         target != GL_TEXTURE_EXTERNAL_OES);
  uint32_t channels = GLES2Util::GetChannelsForFormat(format);

  bool must_use_gl_clear = false;
  if ((channels & GLES2Util::kDepth) != 0 &&
      feature_info_->feature_flags().angle_depth_texture &&
      feature_info_->gl_version_info().is_es2) {
    must_use_gl_clear = true;
  }

  uint32_t size;
  uint32_t padded_row_size;
  if (!GLES2Util::ComputeImageDataSizes(width, height, 1, format, type,
                                        state_.unpack_alignment, &size, nullptr,
                                        &padded_row_size)) {
    return false;
  }

  bool prefer_use_gl_clear = false;
#if defined(OS_MACOSX)
  const uint32_t kMinSizeForGLClear = 4 * 1024;
  prefer_use_gl_clear = size > kMinSizeForGLClear;
#endif
  if (must_use_gl_clear || prefer_use_gl_clear) {
    if (ClearLevelUsingGL(texture, channels, target, level, xoffset, yoffset,
                          width, height)) {
      return true;
    }
  }
  if (must_use_gl_clear)
    return false;

  TRACE_EVENT1("gpu", "Clear using TexSubImage2D", "size", size);

  int tile_height;

  const uint32_t kMaxZeroSize = 1024 * 1024 * 4;
  if (size > kMaxZeroSize) {
    if (kMaxZeroSize < padded_row_size) {
      return false;
    }
    DCHECK_GT(padded_row_size, 0U);
    tile_height = kMaxZeroSize / padded_row_size;
    if (!GLES2Util::ComputeImageDataSizes(width, tile_height, 1, format, type,
                                          state_.unpack_alignment, &size,
                                          nullptr, nullptr)) {
      return false;
    }
  } else {
    tile_height = height;
  }

  api()->glBindTextureFn(texture->target(), texture->service_id());
  {
    std::unique_ptr<char[]> zero(new char[size]);
    memset(zero.get(), 0, size);

    ScopedPixelUnpackState reset_restore(&state_);
    GLint y = 0;
    while (y < height) {
      GLint h = y + tile_height > height ? height - y : tile_height;
      api()->glTexSubImage2DFn(
          target, level, xoffset, yoffset + y, width, h,
          TextureManager::AdjustTexFormat(feature_info_.get(), format), type,
          zero.get());
      y += tile_height;
    }
  }

  TextureRef* bound_texture =
      texture_manager()->GetTextureInfoForTarget(&state_, texture->target());
  api()->glBindTextureFn(texture->target(),
                         bound_texture ? bound_texture->service_id() : 0);
  DCHECK(glGetError() == GL_NO_ERROR);
  return true;
}
