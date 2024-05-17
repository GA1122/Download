void GLES2DecoderImpl::ReturnFrontBuffer(const Mailbox& mailbox, bool is_lost) {
  TextureBase* texture = mailbox_manager()->ConsumeTexture(mailbox);
  mailbox_manager()->TextureDeleted(texture);

  if (offscreen_single_buffer_)
    return;

  for (auto it = saved_back_textures_.begin(); it != saved_back_textures_.end();
       ++it) {
    if (texture != it->back_texture->texture_ref()->texture())
      continue;

    if (is_lost || it->back_texture->size() != offscreen_size_) {
      if (is_lost)
        it->back_texture->Invalidate();
      else
        it->back_texture->Destroy();
      saved_back_textures_.erase(it);
      return;
    }

    it->in_use = false;
    return;
  }

  DLOG(ERROR) << "Attempting to return a frontbuffer that was not saved.";
}
