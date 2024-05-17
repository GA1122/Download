void GLES2DecoderImpl::DoSwapBuffers() {
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();

  int this_frame_number = frame_number_++;
  TRACE_EVENT_INSTANT2("test_gpu", "SwapBuffersLatency",
                       TRACE_EVENT_SCOPE_THREAD,
                       "GLImpl", static_cast<int>(gfx::GetGLImplementation()),
                       "width", (is_offscreen ? offscreen_size_.width() :
                                 surface_->GetSize().width()));
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoSwapBuffers",
               "offscreen", is_offscreen,
               "frame", this_frame_number);
  {
    TRACE_EVENT_SYNTHETIC_DELAY("gpu.PresentingFrame");
  }

  bool is_tracing;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("gpu.debug"),
                                     &is_tracing);
  if (is_tracing) {
    ScopedFrameBufferBinder binder(this, GetBackbufferServiceId());
    gpu_state_tracer_->TakeSnapshotWithCurrentFramebuffer(
        is_offscreen ? offscreen_size_ : surface_->GetSize());
  }

  if (is_offscreen) {
    TRACE_EVENT2("gpu", "Offscreen",
        "width", offscreen_size_.width(), "height", offscreen_size_.height());
    if (offscreen_size_ != offscreen_saved_color_texture_->size()) {
      if (workarounds().needs_offscreen_buffer_workaround) {
        offscreen_saved_frame_buffer_->Create();
        glFinish();
      }

      DCHECK(offscreen_saved_color_format_);
      offscreen_saved_color_texture_->AllocateStorage(
          offscreen_size_, offscreen_saved_color_format_, false);

      offscreen_saved_frame_buffer_->AttachRenderTexture(
          offscreen_saved_color_texture_.get());
      if (offscreen_size_.width() != 0 && offscreen_size_.height() != 0) {
        if (offscreen_saved_frame_buffer_->CheckStatus() !=
            GL_FRAMEBUFFER_COMPLETE) {
          LOG(ERROR) << "GLES2DecoderImpl::ResizeOffscreenFrameBuffer failed "
                     << "because offscreen saved FBO was incomplete.";
          LoseContext(GL_UNKNOWN_CONTEXT_RESET_ARB);
          return;
        }

        {
          ScopedFrameBufferBinder binder(this,
                                         offscreen_saved_frame_buffer_->id());
          glClearColor(0, 0, 0, 0);
          state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
          state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
          glClear(GL_COLOR_BUFFER_BIT);
          RestoreClearState();
        }
      }

      UpdateParentTextureInfo();
    }

    if (offscreen_size_.width() == 0 || offscreen_size_.height() == 0)
      return;
    ScopedGLErrorSuppressor suppressor(
        "GLES2DecoderImpl::DoSwapBuffers", GetErrorState());

    if (IsOffscreenBufferMultisampled()) {
      ScopedResolvedFrameBufferBinder binder(this, true, false);
    } else {
      ScopedFrameBufferBinder binder(this,
                                     offscreen_target_frame_buffer_->id());

      if (offscreen_target_buffer_preserved_) {
        offscreen_saved_color_texture_->Copy(
            offscreen_saved_color_texture_->size(),
            offscreen_saved_color_format_);
      } else {
        if (!!offscreen_saved_color_texture_info_.get())
          offscreen_saved_color_texture_info_->texture()->
              SetServiceId(offscreen_target_color_texture_->id());

        offscreen_saved_color_texture_.swap(offscreen_target_color_texture_);
        offscreen_target_frame_buffer_->AttachRenderTexture(
            offscreen_target_color_texture_.get());
      }

      if (!feature_info_->feature_flags().is_angle)
        glFlush();
    }
  } else {
    if (!surface_->SwapBuffers()) {
      LOG(ERROR) << "Context lost because SwapBuffers failed.";
      LoseContext(GL_UNKNOWN_CONTEXT_RESET_ARB);
    }
  }
}
