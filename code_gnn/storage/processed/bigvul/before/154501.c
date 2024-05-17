bool GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer(
    const gfx::Size& size) {
  DCHECK(offscreen_);
  if (!emulated_back_buffer_) {
    LOG(ERROR)
        << "GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer called "
        << " with an onscreen framebuffer.";
    return false;
  }

  if (emulated_back_buffer_->size == size) {
    return true;
  }

  if (size.width() < 0 || size.height() < 0 ||
      size.width() > max_offscreen_framebuffer_size_ ||
      size.height() > max_offscreen_framebuffer_size_) {
    LOG(ERROR) << "GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer "
                  "failed to allocate storage due to excessive dimensions.";
    return false;
  }

  CheckErrorCallbackState();

  if (!emulated_back_buffer_->Resize(size, feature_info_.get())) {
    LOG(ERROR) << "GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer "
                  "failed to resize the emulated framebuffer.";
    return false;
  }

  if (CheckErrorCallbackState()) {
    LOG(ERROR) << "GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer "
                  "failed to resize the emulated framebuffer because errors "
                  "were generated.";
    return false;
  }

  for (auto& available_color_texture : available_color_textures_) {
    DCHECK(available_color_texture->size != size);
    available_color_texture->Destroy(true);
  }
  available_color_textures_.clear();

  return true;
}
