bool GLES2DecoderImpl::SetParent(GLES2Decoder* new_parent,
                                 uint32 new_parent_texture_id) {
  if (!offscreen_saved_color_texture_.get())
    return false;

  if (parent_) {
    GLuint service_id = offscreen_saved_color_texture_->id();
    GLuint client_id = 0;
    if (parent_->texture_manager()->GetClientId(service_id, &client_id)) {
      parent_->texture_manager()->RemoveTextureInfo(feature_info_, client_id);
    }
  }

  GLES2DecoderImpl* new_parent_impl = static_cast<GLES2DecoderImpl*>(
      new_parent);
  if (new_parent_impl) {
    GLuint service_id = offscreen_saved_color_texture_->id();

    if (new_parent_impl->texture_manager()->GetTextureInfo(
            new_parent_texture_id))
      new_parent_impl->texture_manager()->RemoveTextureInfo(
          feature_info_, new_parent_texture_id);

    TextureManager::TextureInfo* info =
        new_parent_impl->CreateTextureInfo(new_parent_texture_id, service_id);
    info->SetNotOwned();
    new_parent_impl->texture_manager()->SetInfoTarget(info, GL_TEXTURE_2D);

    parent_ = new_parent_impl->AsWeakPtr();

    UpdateParentTextureInfo();
  } else {
    parent_.reset();
  }

  return true;
}
