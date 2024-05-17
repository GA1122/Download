void GLES2DecoderImpl::UpdateParentTextureInfo() {
  if (parent_) {
    TextureManager* parent_texture_manager = parent_->texture_manager();
    parent_texture_manager->SetLevelInfo(
        offscreen_saved_color_texture_info_,
        GL_TEXTURE_2D,
        0,   
        GL_RGBA,
        offscreen_size_.width(),
        offscreen_size_.height(),
        1,   
        0,   
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        true);
    parent_texture_manager->SetParameter(
        offscreen_saved_color_texture_info_,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    parent_texture_manager->SetParameter(
        offscreen_saved_color_texture_info_,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);
    parent_texture_manager->SetParameter(
        offscreen_saved_color_texture_info_,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    parent_texture_manager->SetParameter(
        offscreen_saved_color_texture_info_,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);
  } else {
    offscreen_saved_color_texture_info_ = NULL;
  }
}
