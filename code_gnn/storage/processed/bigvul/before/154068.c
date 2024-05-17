void GLES2DecoderImpl::DoSwapBuffers(uint64_t swap_id, GLbitfield flags) {
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();

  int this_frame_number = frame_number_++;
  TRACE_EVENT_INSTANT2(
      "test_gpu", "SwapBuffersLatency", TRACE_EVENT_SCOPE_THREAD, "GLImpl",
      static_cast<int>(gl::GetGLImplementation()), "width",
      (is_offscreen ? offscreen_size_.width() : surface_->GetSize().width()));
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoSwapBuffers",
               "offscreen", is_offscreen,
               "frame", this_frame_number);

  ScopedGPUTrace scoped_gpu_trace(gpu_tracer_.get(), kTraceDecoder,
                                  "GLES2Decoder", "SwapBuffer");

  bool is_tracing;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("gpu.debug"),
                                     &is_tracing);
  if (is_tracing) {
    ScopedFramebufferBinder binder(this, GetBoundDrawFramebufferServiceId());
    gpu_state_tracer_->TakeSnapshotWithCurrentFramebuffer(
        is_offscreen ? offscreen_size_ : surface_->GetSize());
  }

  ClearScheduleCALayerState();

  if (is_offscreen) {
    TRACE_EVENT2("gpu", "Offscreen",
        "width", offscreen_size_.width(), "height", offscreen_size_.height());

    if (offscreen_single_buffer_)
      return;

    if (offscreen_size_ != offscreen_saved_color_texture_->size()) {
      if (workarounds().needs_offscreen_buffer_workaround) {
        offscreen_saved_frame_buffer_->Create();
        api()->glFinishFn();
      }

      ReleaseNotInUseBackTextures();

      DCHECK(offscreen_saved_color_format_);
      offscreen_saved_color_texture_->AllocateStorage(
          offscreen_size_, offscreen_saved_color_format_, false);

      offscreen_saved_frame_buffer_->AttachRenderTexture(
          offscreen_saved_color_texture_.get());
      if (offscreen_size_.width() != 0 && offscreen_size_.height() != 0) {
        if (offscreen_saved_frame_buffer_->CheckStatus() !=
            GL_FRAMEBUFFER_COMPLETE) {
          LOG(ERROR) << "GLES2DecoderImpl::ResizeOffscreenFramebuffer failed "
                     << "because offscreen saved FBO was incomplete.";
          MarkContextLost(error::kUnknown);
          group_->LoseContexts(error::kUnknown);
          return;
        }

        {
          ScopedFramebufferBinder binder(this,
                                         offscreen_saved_frame_buffer_->id());
          api()->glClearColorFn(0, 0, 0, BackBufferAlphaClearColor());
          state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
          state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
          ClearDeviceWindowRectangles();
          api()->glClearFn(GL_COLOR_BUFFER_BIT);
          RestoreClearState();
        }
      }
    }

    if (offscreen_size_.width() == 0 || offscreen_size_.height() == 0)
      return;
    ScopedGLErrorSuppressor suppressor("GLES2DecoderImpl::DoSwapBuffers",
                                       error_state_.get());

    if (IsOffscreenBufferMultisampled()) {
      ScopedResolvedFramebufferBinder binder(this, true, false);
    } else {
      ScopedFramebufferBinder binder(this,
                                     offscreen_target_frame_buffer_->id());

      if (offscreen_target_buffer_preserved_) {
        offscreen_saved_color_texture_->Copy();
      } else {
        offscreen_saved_color_texture_.swap(offscreen_target_color_texture_);
        offscreen_target_frame_buffer_->AttachRenderTexture(
            offscreen_target_color_texture_.get());
        offscreen_saved_frame_buffer_->AttachRenderTexture(
            offscreen_saved_color_texture_.get());
      }

      if (!gl_version_info().is_angle)
        api()->glFlushFn();
    }
  } else if (supports_async_swap_) {
    TRACE_EVENT_ASYNC_BEGIN0("gpu", "AsyncSwapBuffers", swap_id);

    client()->OnSwapBuffers(swap_id, flags);
    surface_->SwapBuffersAsync(
        base::BindOnce(&GLES2DecoderImpl::FinishAsyncSwapBuffers,
                       weak_ptr_factory_.GetWeakPtr(), swap_id),
        base::DoNothing());
  } else {
    client()->OnSwapBuffers(swap_id, flags);
    FinishSwapBuffers(surface_->SwapBuffers(base::DoNothing()));
  }

  ExitCommandProcessingEarly();
}
