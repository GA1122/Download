void RenderFrameSubmissionObserver::WaitForAnyFrameSubmission() {
  break_on_any_frame_ = true;
  Wait();
  break_on_any_frame_ = false;
}
