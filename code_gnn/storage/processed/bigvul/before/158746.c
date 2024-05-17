ScopedResolvedFramebufferBinder::ScopedResolvedFramebufferBinder(
    GLES2DecoderImpl* decoder, bool enforce_internal_framebuffer, bool internal)
    : decoder_(decoder) {
  resolve_and_bind_ = (
      decoder_->offscreen_target_frame_buffer_.get() &&
      decoder_->IsOffscreenBufferMultisampled() &&
      (!decoder_->framebuffer_state_.bound_read_framebuffer.get() ||
       enforce_internal_framebuffer));
  if (!resolve_and_bind_)
    return;
  auto* api = decoder_->api();
  ScopedGLErrorSuppressor suppressor(
      "ScopedResolvedFramebufferBinder::ctor", decoder_->GetErrorState());

  bool alpha_channel_needs_clear =
      decoder_->should_use_native_gmb_for_backbuffer_ &&
      !decoder_->offscreen_buffer_should_have_alpha_ &&
      decoder_->ChromiumImageNeedsRGBEmulation() &&
      decoder_->workarounds()
          .disable_multisampling_color_mask_usage;
  if (alpha_channel_needs_clear) {
    api->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER_EXT,
                                decoder_->offscreen_target_frame_buffer_->id());
    decoder_->state_.SetDeviceColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    decoder->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
    decoder->ClearDeviceWindowRectangles();
    api->glClearColorFn(0, 0, 0, 1);
    api->glClearFn(GL_COLOR_BUFFER_BIT);
    decoder_->RestoreClearState();
  }

  api->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER_EXT,
                              decoder_->offscreen_target_frame_buffer_->id());
  GLuint targetid;
  if (internal) {
    if (!decoder_->offscreen_resolved_frame_buffer_.get()) {
      decoder_->offscreen_resolved_frame_buffer_.reset(
          new BackFramebuffer(decoder_));
      decoder_->offscreen_resolved_frame_buffer_->Create();
      decoder_->offscreen_resolved_color_texture_.reset(
          new BackTexture(decoder));
      decoder_->offscreen_resolved_color_texture_->Create();

      DCHECK(decoder_->offscreen_saved_color_format_);
      decoder_->offscreen_resolved_color_texture_->AllocateStorage(
          decoder_->offscreen_size_, decoder_->offscreen_saved_color_format_,
          false);
      decoder_->offscreen_resolved_frame_buffer_->AttachRenderTexture(
          decoder_->offscreen_resolved_color_texture_.get());
      if (decoder_->offscreen_resolved_frame_buffer_->CheckStatus() !=
          GL_FRAMEBUFFER_COMPLETE) {
        LOG(ERROR) << "ScopedResolvedFramebufferBinder failed "
                   << "because offscreen resolved FBO was incomplete.";
        return;
      }
    }
    targetid = decoder_->offscreen_resolved_frame_buffer_->id();
  } else {
    targetid = decoder_->offscreen_saved_frame_buffer_->id();
  }
  api->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER_EXT, targetid);
  const int width = decoder_->offscreen_size_.width();
  const int height = decoder_->offscreen_size_.height();
  decoder->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
  decoder->ClearDeviceWindowRectangles();
  decoder->api()->glBlitFramebufferFn(0, 0, width, height, 0, 0, width, height,
                                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
  api->glBindFramebufferEXTFn(GL_FRAMEBUFFER, targetid);
}
