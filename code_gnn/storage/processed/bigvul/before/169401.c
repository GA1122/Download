RenderFrameSubmissionObserver::~RenderFrameSubmissionObserver() {
  render_frame_metadata_provider_->RemoveObserver(this);
  render_frame_metadata_provider_->ReportAllFrameSubmissionsForTesting(false);
}
