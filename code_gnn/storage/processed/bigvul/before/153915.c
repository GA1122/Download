void GLES2DecoderImpl::CopySubTextureHelper(const char* function_name,
                                            GLuint source_id,
                                            GLint source_level,
                                            GLenum dest_target,
                                            GLuint dest_id,
                                            GLint dest_level,
                                            GLint xoffset,
                                            GLint yoffset,
                                            GLint x,
                                            GLint y,
                                            GLsizei width,
                                            GLsizei height,
                                            GLboolean unpack_flip_y,
                                            GLboolean unpack_premultiply_alpha,
                                            GLboolean unpack_unmultiply_alpha,
                                            GLboolean dither) {
  TextureRef* source_texture_ref = GetTexture(source_id);
  TextureRef* dest_texture_ref = GetTexture(dest_id);

  if (!ValidateCopyTextureCHROMIUMTextures(
          function_name, dest_target, source_texture_ref, dest_texture_ref)) {
    return;
  }

  if (source_level < 0 || dest_level < 0 ||
      (feature_info_->IsWebGL1OrES2Context() && source_level > 0)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "source_level or dest_level out of range");
    return;
  }

  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  GLenum source_target = source_texture->target();
  GLenum dest_binding_target = dest_texture->target();
  int source_width = 0;
  int source_height = 0;
  gl::GLImage* image =
      source_texture->GetLevelImage(source_target, source_level);
  if (image) {
    gfx::Size size = image->GetSize();
    source_width = size.width();
    source_height = size.height();
    if (source_width <= 0 || source_height <= 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid image size");
      return;
    }

    int32_t max_x;
    int32_t max_y;
    if (!base::CheckAdd(x, width).AssignIfValid(&max_x) ||
        !base::CheckAdd(y, height).AssignIfValid(&max_y) || x < 0 || y < 0 ||
        max_x > source_width || max_y > source_height) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "source texture bad dimensions");
      return;
    }
  } else {
    if (!source_texture->GetLevelSize(source_target, source_level,
                                      &source_width, &source_height, nullptr)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "source texture has no data for level");
      return;
    }

    if (!texture_manager()->ValidForTarget(source_target, source_level,
                                           source_width, source_height, 1)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "source texture bad dimensions");
      return;
    }

    if (!source_texture->ValidForTexture(source_target, source_level, x, y, 0,
                                         width, height, 1)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "source texture bad dimensions.");
      return;
    }
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(source_target, source_level, &source_type,
                               &source_internal_format);

  GLenum dest_type = 0;
  GLenum dest_internal_format = 0;
  bool dest_level_defined = dest_texture->GetLevelType(
      dest_target, dest_level, &dest_type, &dest_internal_format);
  if (!dest_level_defined) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "destination texture is not defined");
    return;
  }
  if (!dest_texture->ValidForTexture(dest_target, dest_level, xoffset, yoffset,
                                     0, width, height, 1)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "destination texture bad dimensions.");
    return;
  }

  std::string output_error_msg;
  if (!ValidateCopyTextureCHROMIUMInternalFormats(
          GetFeatureInfo(), source_internal_format, dest_internal_format,
          &output_error_msg)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       output_error_msg.c_str());
    return;
  }

  if (feature_info_->feature_flags().desktop_srgb_support) {
    bool enable_framebuffer_srgb =
        GLES2Util::GetColorEncodingFromInternalFormat(source_internal_format) ==
            GL_SRGB ||
        GLES2Util::GetColorEncodingFromInternalFormat(dest_internal_format) ==
            GL_SRGB;
    state_.EnableDisableFramebufferSRGB(enable_framebuffer_srgb);
  }

  if (!texture_manager()->ClearTextureLevel(this, source_texture_ref,
                                            source_target, source_level)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, function_name,
                       "source texture dimensions too big");
    return;
  }

  if (!InitializeCopyTextureCHROMIUM(function_name))
    return;

  int dest_width = 0;
  int dest_height = 0;
  bool ok = dest_texture->GetLevelSize(dest_target, dest_level, &dest_width,
                                       &dest_height, nullptr);
  DCHECK(ok);
  if (xoffset != 0 || yoffset != 0 || width != dest_width ||
      height != dest_height) {
    gfx::Rect cleared_rect;
    if (TextureManager::CombineAdjacentRects(
            dest_texture->GetLevelClearedRect(dest_target, dest_level),
            gfx::Rect(xoffset, yoffset, width, height), &cleared_rect)) {
      DCHECK_GE(cleared_rect.size().GetArea(),
                dest_texture->GetLevelClearedRect(dest_target, dest_level)
                    .size()
                    .GetArea());
      texture_manager()->SetLevelClearedRect(dest_texture_ref, dest_target,
                                             dest_level, cleared_rect);
    } else {
      if (!texture_manager()->ClearTextureLevel(this, dest_texture_ref,
                                                dest_target, dest_level)) {
        LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, function_name,
                           "destination texture dimensions too big");
        return;
      }
    }
  } else {
    texture_manager()->SetLevelCleared(dest_texture_ref, dest_target,
                                       dest_level, true);
  }

  bool unpack_premultiply_alpha_change =
      (unpack_premultiply_alpha ^ unpack_unmultiply_alpha) != 0;
  if (image && dest_internal_format == source_internal_format &&
      dest_level == 0 && !unpack_flip_y && !unpack_premultiply_alpha_change &&
      !dither) {
    ScopedTextureBinder binder(&state_, error_state_.get(),
                               dest_texture->service_id(), dest_binding_target);
    if (image->CopyTexSubImage(dest_target, gfx::Point(xoffset, yoffset),
                               gfx::Rect(x, y, width, height))) {
      return;
    }
  }

  DoBindOrCopyTexImageIfNeeded(source_texture, source_target, 0);

  if (source_target == GL_TEXTURE_EXTERNAL_OES) {
    if (GLStreamTextureImage* image =
            source_texture->GetLevelStreamTextureImage(GL_TEXTURE_EXTERNAL_OES,
                                                       source_level)) {
      GLfloat transform_matrix[16];
      image->GetTextureMatrix(transform_matrix);
      copy_texture_chromium_->DoCopySubTextureWithTransform(
          this, source_target, source_texture->service_id(), source_level,
          source_internal_format, dest_target, dest_texture->service_id(),
          dest_level, dest_internal_format, xoffset, yoffset, x, y, width,
          height, dest_width, dest_height, source_width, source_height,
          unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
          unpack_unmultiply_alpha == GL_TRUE, dither == GL_TRUE,
          transform_matrix, copy_tex_image_blit_.get());
      return;
    }
  }

  CopyTextureMethod method = GetCopyTextureCHROMIUMMethod(
      GetFeatureInfo(), source_target, source_level, source_internal_format,
      source_type, dest_binding_target, dest_level, dest_internal_format,
      unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
      unpack_unmultiply_alpha == GL_TRUE, dither == GL_TRUE);
#if defined(OS_CHROMEOS) && defined(ARCH_CPU_X86_FAMILY)
  if (Texture::ColorRenderable(GetFeatureInfo(), dest_internal_format,
                               dest_texture->IsImmutable()) &&
      method == CopyTextureMethod::DIRECT_COPY) {
    method = CopyTextureMethod::DIRECT_DRAW;
  }
#endif

  copy_texture_chromium_->DoCopySubTexture(
      this, source_target, source_texture->service_id(), source_level,
      source_internal_format, dest_target, dest_texture->service_id(),
      dest_level, dest_internal_format, xoffset, yoffset, x, y, width, height,
      dest_width, dest_height, source_width, source_height,
      unpack_flip_y == GL_TRUE, unpack_premultiply_alpha == GL_TRUE,
      unpack_unmultiply_alpha == GL_TRUE, dither == GL_TRUE, method,
      copy_tex_image_blit_.get());
}
