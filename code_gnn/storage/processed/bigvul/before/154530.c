void GLES2DecoderPassthroughImpl::TakeFrontBuffer(const Mailbox& mailbox) {
  if (offscreen_single_buffer_) {
    DCHECK(emulated_back_buffer_->color_texture != nullptr);
    mailbox_manager_->ProduceTexture(
        mailbox, emulated_back_buffer_->color_texture->texture.get());
    return;
  }

  if (!emulated_front_buffer_) {
    DLOG(ERROR) << "Called TakeFrontBuffer on a non-offscreen context";
    return;
  }

  mailbox_manager_->ProduceTexture(mailbox,
                                   emulated_front_buffer_->texture.get());
  in_use_color_textures_.push_back(std::move(emulated_front_buffer_));
  emulated_front_buffer_ = nullptr;

  if (available_color_textures_.empty()) {
    emulated_front_buffer_ = std::make_unique<EmulatedColorBuffer>(
        api(), emulated_default_framebuffer_format_);
    emulated_front_buffer_->Resize(emulated_back_buffer_->size);
    create_color_buffer_count_for_test_++;
  } else {
    emulated_front_buffer_ = std::move(available_color_textures_.back());
    available_color_textures_.pop_back();
  }
}
