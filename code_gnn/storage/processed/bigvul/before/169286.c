void RenderFrameSubmissionObserver::OnRenderFrameSubmission() {
  render_frame_count_++;
  if (break_on_any_frame_)
    Quit();
}
