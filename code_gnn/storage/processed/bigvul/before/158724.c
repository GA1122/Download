error::Error GLES2DecoderImpl::HandlePostSubBufferCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::PostSubBufferCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::PostSubBufferCHROMIUM*>(
          cmd_data);
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandlePostSubBufferCHROMIUM");
  if (!supports_post_sub_buffer_) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glPostSubBufferCHROMIUM", "command not supported by surface");
    return error::kNoError;
  }
  bool is_tracing;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("gpu.debug"),
                                     &is_tracing);
  if (is_tracing) {
    bool is_offscreen = !!offscreen_target_frame_buffer_.get();
    ScopedFramebufferBinder binder(this, GetBoundDrawFramebufferServiceId());
    gpu_state_tracer_->TakeSnapshotWithCurrentFramebuffer(
        is_offscreen ? offscreen_size_ : surface_->GetSize());
  }

  ClearScheduleCALayerState();
  ClearScheduleDCLayerState();

  if (supports_async_swap_) {
    TRACE_EVENT_ASYNC_BEGIN0("gpu", "AsyncSwapBuffers", c.swap_id());

    client_->OnSwapBuffers(c.swap_id(), c.flags);
    surface_->PostSubBufferAsync(
        c.x, c.y, c.width, c.height,
        base::Bind(&GLES2DecoderImpl::FinishAsyncSwapBuffers,
                   weak_ptr_factory_.GetWeakPtr(), c.swap_id()),
        base::DoNothing());
  } else {
    client_->OnSwapBuffers(c.swap_id(), c.flags);
    FinishSwapBuffers(surface_->PostSubBuffer(c.x, c.y, c.width, c.height,
                                              base::DoNothing()));
  }

  return error::kNoError;
}
