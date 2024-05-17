error::Error GLES2DecoderImpl::DoCompressedTexImage(
    GLenum target, GLint level, GLenum internal_format, GLsizei width,
    GLsizei height, GLsizei depth, GLint border, GLsizei image_size,
    const void* data, ContextState::Dimension dimension) {
  const char* func_name;
  if (dimension == ContextState::k2D) {
    func_name = "glCompressedTexImage2D";
    if (!validators_->texture_target.IsValid(target)) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
      return error::kNoError;
    }
    if (target == GL_TEXTURE_RECTANGLE_ARB) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
      return error::kNoError;
    }
  } else {
    DCHECK_EQ(ContextState::k3D, dimension);
    func_name = "glCompressedTexImage3D";
    if (!validators_->texture_3_d_target.IsValid(target)) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
      return error::kNoError;
    }
  }
  if (!validators_->compressed_texture_format.IsValid(internal_format)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(
        func_name, internal_format, "internalformat");
    return error::kNoError;
  }
  if (image_size < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "imageSize < 0");
    return error::kNoError;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, depth) ||
      border != 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "dimensions out of range");
    return error::kNoError;
  }
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, func_name, "no texture bound at target");
    return error::kNoError;
  }
  Texture* texture = texture_ref->texture();
  if (texture->IsImmutable()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "texture is immutable");
    return error::kNoError;
  }

  if (!ValidateCompressedTexDimensions(func_name, target, level,
                                       width, height, depth, internal_format) ||
      !ValidateCompressedTexFuncData(func_name, width, height, depth,
                                     internal_format, image_size, data)) {
    return error::kNoError;
  }

  if (texture->IsAttachedToFramebuffer()) {
    framebuffer_state_.clear_state_dirty = true;
  }

  std::unique_ptr<int8_t[]> zero;
  if (!state_.bound_pixel_unpack_buffer && !data) {
    zero.reset(new int8_t[image_size]);
    memset(zero.get(), 0, image_size);
    data = zero.get();
  }
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(func_name);
  const CompressedFormatInfo* format_info =
      GetCompressedFormatInfo(internal_format);
  if (format_info != nullptr && !format_info->support_check(*feature_info_)) {
    std::unique_ptr<uint8_t[]> decompressed_data = DecompressTextureData(
        state_, *format_info, width, height, depth, image_size, data);
    if (!decompressed_data) {
      MarkContextLost(error::kGuilty);
      group_->LoseContexts(error::kInnocent);
      return error::kLostContext;
    }
    ScopedPixelUnpackState reset_restore(&state_);
    if (dimension == ContextState::k2D) {
      api()->glTexImage2DFn(
          target, level, format_info->decompressed_internal_format, width,
          height, border, format_info->decompressed_format,
          format_info->decompressed_type, decompressed_data.get());
    } else {
      api()->glTexImage3DFn(
          target, level, format_info->decompressed_internal_format, width,
          height, depth, border, format_info->decompressed_format,
          format_info->decompressed_type, decompressed_data.get());
    }
  } else {
    if (dimension == ContextState::k2D) {
      api()->glCompressedTexImage2DFn(target, level, internal_format, width,
                                      height, border, image_size, data);
    } else {
      api()->glCompressedTexImage3DFn(target, level, internal_format, width,
                                      height, depth, border, image_size, data);
    }
  }
  GLenum error = LOCAL_PEEK_GL_ERROR(func_name);
  if (error == GL_NO_ERROR) {
    texture_manager()->SetLevelInfo(texture_ref, target, level, internal_format,
                                    width, height, depth, border, 0, 0,
                                    gfx::Rect(width, height));
  }

  ExitCommandProcessingEarly();
  return error::kNoError;
}
