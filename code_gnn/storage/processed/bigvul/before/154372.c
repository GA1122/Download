void GLES2DecoderImpl::TakeFrontBuffer(const Mailbox& mailbox) {
  if (offscreen_single_buffer_) {
    mailbox_manager()->ProduceTexture(
        mailbox, offscreen_target_color_texture_->texture_ref()->texture());
    return;
  }

  if (!offscreen_saved_color_texture_.get()) {
    DLOG(ERROR) << "Called TakeFrontBuffer on a non-offscreen context";
    return;
  }

  mailbox_manager()->ProduceTexture(
      mailbox, offscreen_saved_color_texture_->texture_ref()->texture());

  SavedBackTexture save;
  save.back_texture.swap(offscreen_saved_color_texture_);
  save.in_use = true;
  saved_back_textures_.push_back(std::move(save));

  CreateBackTexture();
}
