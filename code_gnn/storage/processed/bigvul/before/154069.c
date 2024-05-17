void GLES2DecoderImpl::DoSwapBuffersWithBoundsCHROMIUM(
    uint64_t swap_id,
    GLsizei count,
    const volatile GLint* rects,
    GLbitfield flags) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::SwapBuffersWithBoundsCHROMIUM");
  if (!supports_swap_buffers_with_bounds_) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glSwapBuffersWithBoundsCHROMIUM",
                       "command not supported by surface");
    return;
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

  std::vector<gfx::Rect> bounds(count);
  for (GLsizei i = 0; i < count; ++i) {
    bounds[i] = gfx::Rect(rects[i * 4 + 0], rects[i * 4 + 1], rects[i * 4 + 2],
                          rects[i * 4 + 3]);
  }
  client()->OnSwapBuffers(swap_id, flags);
  FinishSwapBuffers(surface_->SwapBuffersWithBounds(bounds, base::DoNothing()));
}
