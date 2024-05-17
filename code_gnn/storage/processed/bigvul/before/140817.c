error::Error GLES2DecoderImpl::HandlePostSubBufferCHROMIUM(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::PostSubBufferCHROMIUM& c =
      *static_cast<const gles2::cmds::PostSubBufferCHROMIUM*>(cmd_data);
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandlePostSubBufferCHROMIUM");
  {
    TRACE_EVENT_SYNTHETIC_DELAY("gpu.PresentingFrame");
  }
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
    ScopedFrameBufferBinder binder(this, GetBackbufferServiceId());
    gpu_state_tracer_->TakeSnapshotWithCurrentFramebuffer(
        is_offscreen ? offscreen_size_ : surface_->GetSize());
  }
  if (surface_->PostSubBuffer(c.x, c.y, c.width, c.height)) {
    return error::kNoError;
  } else {
    LOG(ERROR) << "Context lost because PostSubBuffer failed.";
    return error::kLostContext;
  }
}
