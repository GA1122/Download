error::Error GLES2DecoderPassthroughImpl::DoSwapBuffers(uint64_t swap_id,
                                                        GLbitfield flags) {
  if (offscreen_) {
    if (offscreen_single_buffer_) {
      return error::kNoError;
    }

    DCHECK(emulated_back_buffer_);

    if (emulated_front_buffer_ &&
        emulated_front_buffer_->size != emulated_back_buffer_->size) {
      emulated_front_buffer_->Destroy(true);
      emulated_front_buffer_ = nullptr;
    }

    if (emulated_front_buffer_ == nullptr) {
      if (!available_color_textures_.empty()) {
        emulated_front_buffer_ = std::move(available_color_textures_.back());
        available_color_textures_.pop_back();
      } else {
        emulated_front_buffer_ = std::make_unique<EmulatedColorBuffer>(
            api(), emulated_default_framebuffer_format_);
        emulated_front_buffer_->Resize(emulated_back_buffer_->size);
      }
    }

    DCHECK(emulated_front_buffer_->size == emulated_back_buffer_->size);

    if (emulated_default_framebuffer_format_.samples > 0) {
      emulated_back_buffer_->Blit(emulated_front_buffer_.get());
    } else {
      DCHECK(emulated_back_buffer_->color_texture != nullptr);
      if (offscreen_target_buffer_preserved_) {
        emulated_back_buffer_->Blit(emulated_front_buffer_.get());
      }

      std::unique_ptr<EmulatedColorBuffer> old_front_buffer =
          std::move(emulated_front_buffer_);
      emulated_front_buffer_ =
          emulated_back_buffer_->SetColorBuffer(std::move(old_front_buffer));
    }

    return error::kNoError;
  }

  client()->OnSwapBuffers(swap_id, flags);
  return CheckSwapBuffersResult(surface_->SwapBuffers(base::DoNothing()),
                                "SwapBuffers");
}
