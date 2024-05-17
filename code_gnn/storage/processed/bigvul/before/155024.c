void WebGLRenderingContextBase::deleteTexture(WebGLTexture* texture) {
  if (!DeleteObject(texture))
    return;

  int max_bound_texture_index = -1;
  for (wtf_size_t i = 0; i < one_plus_max_non_default_texture_unit_; ++i) {
    if (texture == texture_units_[i].texture2d_binding_) {
      texture_units_[i].texture2d_binding_ = nullptr;
      max_bound_texture_index = i;
    }
    if (texture == texture_units_[i].texture_cube_map_binding_) {
      texture_units_[i].texture_cube_map_binding_ = nullptr;
      max_bound_texture_index = i;
    }
    if (IsWebGL2OrHigher()) {
      if (texture == texture_units_[i].texture3d_binding_) {
        texture_units_[i].texture3d_binding_ = nullptr;
        max_bound_texture_index = i;
      }
      if (texture == texture_units_[i].texture2d_array_binding_) {
        texture_units_[i].texture2d_array_binding_ = nullptr;
        max_bound_texture_index = i;
      }
    }
  }
  if (framebuffer_binding_)
    framebuffer_binding_->RemoveAttachmentFromBoundFramebuffer(GL_FRAMEBUFFER,
                                                               texture);
  if (GetFramebufferBinding(GL_READ_FRAMEBUFFER))
    GetFramebufferBinding(GL_READ_FRAMEBUFFER)
        ->RemoveAttachmentFromBoundFramebuffer(GL_READ_FRAMEBUFFER, texture);

  if (one_plus_max_non_default_texture_unit_ ==
      static_cast<wtf_size_t>(max_bound_texture_index + 1)) {
    FindNewMaxNonDefaultTextureUnit();
  }
}
