void WebGLRenderingContextBase::bindTexture(GLenum target,
                                            WebGLTexture* texture) {
  if (!ValidateNullableWebGLObject("bindTexture", texture))
    return;
  if (texture && texture->GetTarget() && texture->GetTarget() != target) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindTexture",
                      "textures can not be used with multiple targets");
    return;
  }

  if (target == GL_TEXTURE_2D) {
    texture_units_[active_texture_unit_].texture2d_binding_ = texture;
  } else if (target == GL_TEXTURE_CUBE_MAP) {
    texture_units_[active_texture_unit_].texture_cube_map_binding_ = texture;
  } else if (IsWebGL2OrHigher() && target == GL_TEXTURE_2D_ARRAY) {
    texture_units_[active_texture_unit_].texture2d_array_binding_ = texture;
  } else if (IsWebGL2OrHigher() && target == GL_TEXTURE_3D) {
    texture_units_[active_texture_unit_].texture3d_binding_ = texture;
  } else if (target == GL_TEXTURE_VIDEO_IMAGE_WEBGL) {
    if (!ExtensionEnabled(kWebGLVideoTextureName)) {
      SynthesizeGLError(
          GL_INVALID_VALUE, "bindTexture",
          "unhandled type, WEBGL_video_texture extension not enabled");
      return;
    }
    texture_units_[active_texture_unit_].texture_video_image_binding_ = texture;
  } else {
    SynthesizeGLError(GL_INVALID_ENUM, "bindTexture", "invalid target");
    return;
  }

  if (target == GL_TEXTURE_VIDEO_IMAGE_WEBGL) {
#if defined(OS_ANDROID)
    ContextGL()->BindTexture(GL_TEXTURE_EXTERNAL_OES, ObjectOrZero(texture));
#else
    ContextGL()->BindTexture(GL_TEXTURE_2D, ObjectOrZero(texture));
    if (texture && !texture->GetTarget()) {
      ContextGL()->TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                 GL_LINEAR);
      ContextGL()->TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                 GL_LINEAR);
      ContextGL()->TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                 GL_CLAMP_TO_EDGE);
      ContextGL()->TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                 GL_CLAMP_TO_EDGE);
    }
#endif   
  } else {
    ContextGL()->BindTexture(target, ObjectOrZero(texture));
  }
  if (texture) {
    texture->SetTarget(target);
    one_plus_max_non_default_texture_unit_ =
        max(active_texture_unit_ + 1, one_plus_max_non_default_texture_unit_);
  } else {
    if (one_plus_max_non_default_texture_unit_ == active_texture_unit_ + 1) {
      FindNewMaxNonDefaultTextureUnit();
    }
  }

}
