void GLES2DecoderImpl::UpdateParentTextureInfo() {
  if (parent_) {
    GLuint service_id = offscreen_saved_color_texture_->id();
    GLuint client_id;
    TextureManager* parent_texture_manager = parent_->texture_manager();
    CHECK(parent_texture_manager->GetClientId(service_id, &client_id));
    TextureManager::TextureInfo* info = parent_->GetTextureInfo(client_id);
    DCHECK(info);

    parent_texture_manager->SetLevelInfo(
        feature_info_,
        info,
        GL_TEXTURE_2D,
        0,   
        GL_RGBA,
        offscreen_size_.width(),
        offscreen_size_.height(),
        1,   
        0,   
        GL_RGBA,
        GL_UNSIGNED_BYTE);
    parent_texture_manager->SetParameter(
        feature_info_,
        info,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    parent_texture_manager->SetParameter(
        feature_info_,
        info,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);
    parent_texture_manager->SetParameter(
        feature_info_,
        info,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    parent_texture_manager->SetParameter(
        feature_info_,
        info,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);
  }
}
