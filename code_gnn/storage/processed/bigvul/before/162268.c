void CommandBufferProxyImpl::OnBufferPresented(
    uint64_t swap_id,
    const gfx::PresentationFeedback& feedback) {
  DCHECK(gl::IsPresentationCallbackEnabled());
  if (presentation_callback_)
    presentation_callback_.Run(swap_id, feedback);

  if (update_vsync_parameters_completion_callback_ &&
      feedback.timestamp != base::TimeTicks()) {
    update_vsync_parameters_completion_callback_.Run(feedback.timestamp,
                                                     feedback.interval);
  }
}
