void WebGLRenderingContextBase::bindTexture(GLenum target,
                                            WebGLTexture* texture) {
  bool deleted;
  if (!CheckObjectToBeBound("bindTexture", texture, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindTexture",
                      "attempt to bind a deleted texture");
    return;
  }
  if (texture && texture->GetTarget() && texture->GetTarget() != target) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindTexture",
                      "textures can not be used with multiple targets");
    return;
  }

  if (target == GL_TEXTURE_2D) {
    texture_units_[active_texture_unit_].texture2d_binding_ =
        TraceWrapperMember<WebGLTexture>(this, texture);
  } else if (target == GL_TEXTURE_CUBE_MAP) {
    texture_units_[active_texture_unit_].texture_cube_map_binding_ =
        TraceWrapperMember<WebGLTexture>(this, texture);
  } else if (IsWebGL2OrHigher() && target == GL_TEXTURE_2D_ARRAY) {
    texture_units_[active_texture_unit_].texture2d_array_binding_ =
        TraceWrapperMember<WebGLTexture>(this, texture);
  } else if (IsWebGL2OrHigher() && target == GL_TEXTURE_3D) {
    texture_units_[active_texture_unit_].texture3d_binding_ =
        TraceWrapperMember<WebGLTexture>(this, texture);
  } else {
    SynthesizeGLError(GL_INVALID_ENUM, "bindTexture", "invalid target");
    return;
  }

  ContextGL()->BindTexture(target, ObjectOrZero(texture));
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
