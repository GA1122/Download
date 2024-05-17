void GLES2DecoderPassthroughImpl::ReturnFrontBuffer(const Mailbox& mailbox,
                                                    bool is_lost) {
  TextureBase* texture = mailbox_manager_->ConsumeTexture(mailbox);
  mailbox_manager_->TextureDeleted(texture);

  if (offscreen_single_buffer_) {
    return;
  }

  auto it = in_use_color_textures_.begin();
  while (it != in_use_color_textures_.end()) {
    if ((*it)->texture == texture) {
      break;
    }
    it++;
  }
  if (it == in_use_color_textures_.end()) {
    DLOG(ERROR) << "Attempting to return a frontbuffer that was not saved.";
    return;
  }

  if (is_lost) {
    (*it)->texture->MarkContextLost();
    (*it)->Destroy(false);
  } else if ((*it)->size != emulated_back_buffer_->size) {
    (*it)->Destroy(true);
  } else {
    available_color_textures_.push_back(std::move(*it));
  }
  in_use_color_textures_.erase(it);
}
