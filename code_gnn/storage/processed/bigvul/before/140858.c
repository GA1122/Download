void GLES2DecoderImpl::UpdateParentTextureInfo() {
  if (!offscreen_saved_color_texture_info_.get())
    return;
  GLenum target = offscreen_saved_color_texture_info_->texture()->target();
  glBindTexture(target, offscreen_saved_color_texture_info_->service_id());
  texture_manager()->SetLevelInfo(
      offscreen_saved_color_texture_info_.get(),
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
  texture_manager()->SetParameteri(
      "UpdateParentTextureInfo",
      GetErrorState(),
      offscreen_saved_color_texture_info_.get(),
      GL_TEXTURE_MAG_FILTER,
      GL_LINEAR);
  texture_manager()->SetParameteri(
      "UpdateParentTextureInfo",
      GetErrorState(),
      offscreen_saved_color_texture_info_.get(),
      GL_TEXTURE_MIN_FILTER,
      GL_LINEAR);
  texture_manager()->SetParameteri(
      "UpdateParentTextureInfo",
      GetErrorState(),
      offscreen_saved_color_texture_info_.get(),
      GL_TEXTURE_WRAP_S,
      GL_CLAMP_TO_EDGE);
  texture_manager()->SetParameteri(
      "UpdateParentTextureInfo",
      GetErrorState(),
      offscreen_saved_color_texture_info_.get(),
      GL_TEXTURE_WRAP_T,
      GL_CLAMP_TO_EDGE);
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  glBindTexture(target, texture_ref ? texture_ref->service_id() : 0);
}
