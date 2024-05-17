void GLES2DecoderImpl::DoGenerateMipmap(GLenum target) {
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref ||
      !texture_manager()->CanGenerateMipmaps(texture_ref)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glGenerateMipmap", "Can not generate mips");
    return;
  }
  Texture* tex = texture_ref->texture();
  GLint base_level = tex->base_level();

  if (target == GL_TEXTURE_CUBE_MAP) {
    for (int i = 0; i < 6; ++i) {
      GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
      if (!texture_manager()->ClearTextureLevel(this, texture_ref, face,
                                                base_level)) {
        LOCAL_SET_GL_ERROR(
            GL_OUT_OF_MEMORY, "glGenerateMipmap", "dimensions too big");
        return;
      }
    }
  } else {
    if (!texture_manager()->ClearTextureLevel(this, texture_ref, target,
                                              base_level)) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY, "glGenerateMipmap", "dimensions too big");
      return;
    }
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glGenerateMipmap");
  bool texture_zero_level_set = false;
  GLenum type = 0;
  GLenum internal_format = 0;
  GLenum format = 0;
  if (workarounds().set_zero_level_before_generating_mipmap &&
      target == GL_TEXTURE_2D) {
    if (base_level != 0 &&
        !tex->GetLevelType(target, 0, &type, &internal_format) &&
        tex->GetLevelType(target, tex->base_level(), &type, &internal_format)) {
      format = TextureManager::ExtractFormatFromStorageFormat(internal_format);
      ScopedPixelUnpackState reset_restore(&state_);
      api()->glTexImage2DFn(target, 0, internal_format, 1, 1, 0, format, type,
                            nullptr);
      texture_zero_level_set = true;
    }
  }

  bool enable_srgb = 0;
  if (target == GL_TEXTURE_2D) {
    tex->GetLevelType(target, tex->base_level(), &type, &internal_format);
    enable_srgb = GLES2Util::GetColorEncodingFromInternalFormat(
                      internal_format) == GL_SRGB;
  }
  if (enable_srgb && feature_info_->feature_flags().desktop_srgb_support) {
    state_.EnableDisableFramebufferSRGB(enable_srgb);
  }
  if (workarounds().clamp_texture_base_level_and_max_level) {
    tex->ApplyClampedBaseLevelAndMaxLevelToDriver();
  }
  if (enable_srgb && workarounds().decode_encode_srgb_for_generatemipmap) {
    if (target == GL_TEXTURE_2D) {
      if (!InitializeSRGBConverter("generateMipmap")) {
        return;
      }
      srgb_converter_->GenerateMipmap(this, tex, target);
    } else {
      api()->glGenerateMipmapEXTFn(target);
    }
  } else {
    api()->glGenerateMipmapEXTFn(target);
  }

  if (texture_zero_level_set) {
    ScopedPixelUnpackState reset_restore(&state_);
    api()->glTexImage2DFn(target, 0, internal_format, 0, 0, 0, format, type,
                          nullptr);
  }

  GLenum error = LOCAL_PEEK_GL_ERROR("glGenerateMipmap");
  if (error == GL_NO_ERROR) {
    texture_manager()->MarkMipmapsGenerated(texture_ref);
  }
}
