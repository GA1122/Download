void Compositor::DidPresentCompositorFrame(
    uint32_t frame_token,
    const gfx::PresentationFeedback& feedback) {
  TRACE_EVENT_MARK_WITH_TIMESTAMP1("cc,benchmark", "FramePresented",
                                   feedback.timestamp, "environment",
                                   trace_environment_name_);
}
