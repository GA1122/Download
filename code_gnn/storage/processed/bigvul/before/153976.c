error::Error GLES2DecoderImpl::DoCompressedTexSubImage(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
    GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    GLsizei image_size, const void * data, ContextState::Dimension dimension) {
  const char* func_name;
  if (dimension == ContextState::k2D) {
    func_name = "glCompressedTexSubImage2D";
    if (!validators_->texture_target.IsValid(target)) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
      return error::kNoError;
    }
  } else {
    DCHECK_EQ(ContextState::k3D, dimension);
    func_name = "glCompressedTexSubImage3D";
    if (!validators_->texture_3_d_target.IsValid(target)) {
      LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
      return error::kNoError;
    }
  }
  if (!validators_->compressed_texture_format.IsValid(format)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, format, "format");
    return error::kNoError;
  }
  if (image_size < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "imageSize < 0");
    return error::kNoError;
  }
  if (!texture_manager()->ValidForTarget(target, level, width, height, depth)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "dimensions out of range");
    return error::kNoError;
  }
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, func_name, "no texture bound at target");
    return error::kNoError;
  }
  Texture* texture = texture_ref->texture();
  GLenum type = 0;
  GLenum internal_format = 0;
  if (!texture->GetLevelType(target, level, &type, &internal_format)) {
    std::string msg = base::StringPrintf("level %d does not exist", level);
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, msg.c_str());
    return error::kNoError;
  }
  if (internal_format != format) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
                       "format does not match internalformat.");
    return error::kNoError;
  }
  if (!texture->ValidForTexture(target, level, xoffset, yoffset, zoffset,
                                width, height, depth)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "bad dimensions.");
    return error::kNoError;
  }

  if (!ValidateCompressedTexFuncData(
          func_name, width, height, depth, format, image_size, data) ||
      !ValidateCompressedTexSubDimensions(
          func_name, target, level, xoffset, yoffset, zoffset,
          width, height, depth, format, texture)) {
    return error::kNoError;
  }
  if (image_size > 0 && !state_.bound_pixel_unpack_buffer && !data)
    return error::kInvalidArguments;

  if (!texture->IsLevelCleared(target, level)) {
    DCHECK(texture->IsImmutable());
    GLsizei level_width = 0, level_height = 0, level_depth = 0;
    bool success = texture->GetLevelSize(
        target, level, &level_width, &level_height, &level_depth);
    DCHECK(success);
    if (xoffset == 0 && width == level_width &&
        yoffset == 0 && height == level_height &&
        zoffset == 0 && depth == level_depth) {
      texture_manager()->SetLevelCleared(texture_ref, target, level, true);
    } else {
      texture_manager()->ClearTextureLevel(this, texture_ref, target, level);
    }
    DCHECK(texture->IsLevelCleared(target, level));
  }

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
      api()->glTexSubImage2DFn(target, level, xoffset, yoffset, width, height,
                               format_info->decompressed_format,
                               format_info->decompressed_type,
                               decompressed_data.get());
    } else {
      api()->glTexSubImage3DFn(target, level, xoffset, yoffset, zoffset, width,
                               height, depth, format_info->decompressed_format,
                               format_info->decompressed_type,
                               decompressed_data.get());
    }
  } else {
    if (dimension == ContextState::k2D) {
      api()->glCompressedTexSubImage2DFn(target, level, xoffset, yoffset, width,
                                         height, format, image_size, data);
    } else {
      api()->glCompressedTexSubImage3DFn(target, level, xoffset, yoffset,
                                         zoffset, width, height, depth, format,
                                         image_size, data);
    }
  }

  ExitCommandProcessingEarly();
  return error::kNoError;
}
