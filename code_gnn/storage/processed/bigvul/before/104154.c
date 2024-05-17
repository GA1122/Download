error::Error GLES2DecoderImpl::HandleSwapBuffers(
    uint32 immediate_data_size, const gles2::SwapBuffers& c) {
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();
  int this_frame_number = frame_number_++;
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::HandleSwapBuffers",
               "offscreen", is_offscreen,
               "frame", this_frame_number);
  if (is_offscreen) {
    if (offscreen_size_ != offscreen_saved_color_texture_->size()) {
      if (needs_mac_nvidia_driver_workaround_) {
        offscreen_saved_frame_buffer_->Create();
        glFinish();
      }

      DCHECK(offscreen_saved_color_format_);
      offscreen_saved_color_texture_->AllocateStorage(
          offscreen_size_, offscreen_saved_color_format_);

      offscreen_saved_frame_buffer_->AttachRenderTexture(
          offscreen_saved_color_texture_.get());
      if (offscreen_saved_frame_buffer_->CheckStatus() !=
          GL_FRAMEBUFFER_COMPLETE) {
        LOG(ERROR) << "GLES2DecoderImpl::ResizeOffscreenFrameBuffer failed "
                   << "because offscreen saved FBO was incomplete.";
        return error::kLostContext;
      }

      {
        ScopedFrameBufferBinder binder(this,
                                       offscreen_saved_frame_buffer_->id());
        glClearColor(0, 0, 0, 0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDisable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        RestoreClearState();
      }

      UpdateParentTextureInfo();
    }

    ScopedGLErrorSuppressor suppressor(this);

    if (IsOffscreenBufferMultisampled()) {
      ScopedResolvedFrameBufferBinder binder(this, true, false);
#if defined(OS_MACOSX)
      if (swap_buffers_callback_.get()) {
        swap_buffers_callback_->Run();
      }
#endif
      return error::kNoError;
    } else {
      ScopedFrameBufferBinder binder(this,
                                     offscreen_target_frame_buffer_->id());

      if (surface_->IsOffscreen()) {
        offscreen_saved_color_texture_->Copy(
            offscreen_saved_color_texture_->size(),
            offscreen_saved_color_format_);

        if (!IsAngle())
          glFlush();
      }

#if defined(OS_MACOSX)
      if (swap_buffers_callback_.get()) {
        swap_buffers_callback_->Run();
      }
#endif
      return error::kNoError;
    }
  } else {
    TRACE_EVENT1("gpu", "GLContext::SwapBuffers", "frame", this_frame_number);
    if (!surface_->SwapBuffers()) {
      LOG(ERROR) << "Context lost because SwapBuffers failed.";
      return error::kLostContext;
    }
  }

#if defined(OS_MACOSX)
  if (swap_buffers_callback_.get()) {
    swap_buffers_callback_->Run();
  }
#endif

  return error::kNoError;
}