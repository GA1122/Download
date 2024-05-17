void GLES2DecoderImpl::CreateBackTexture() {
  for (auto it = saved_back_textures_.begin(); it != saved_back_textures_.end();
       ++it) {
    if (it->in_use)
      continue;

    if (it->back_texture->size() != offscreen_size_)
      continue;
    offscreen_saved_color_texture_ = std::move(it->back_texture);
    offscreen_saved_frame_buffer_->AttachRenderTexture(
        offscreen_saved_color_texture_.get());
    saved_back_textures_.erase(it);
    return;
  }

  ++create_back_texture_count_for_test_;
  offscreen_saved_color_texture_.reset(new BackTexture(this));
  offscreen_saved_color_texture_->Create();
  offscreen_saved_color_texture_->AllocateStorage(
      offscreen_size_, offscreen_saved_color_format_, false);
  offscreen_saved_frame_buffer_->AttachRenderTexture(
      offscreen_saved_color_texture_.get());
}
