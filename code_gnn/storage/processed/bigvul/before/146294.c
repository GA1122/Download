void WebGLRenderingContextBase::FindNewMaxNonDefaultTextureUnit() {
  int start_index = one_plus_max_non_default_texture_unit_ - 1;
  for (int i = start_index; i >= 0; --i) {
    if (texture_units_[i].texture2d_binding_ ||
        texture_units_[i].texture_cube_map_binding_) {
      one_plus_max_non_default_texture_unit_ = i + 1;
      return;
    }
  }
  one_plus_max_non_default_texture_unit_ = 0;
}
