void LocalFrameClientImpl::ProgressEstimateChanged(double progress_estimate) {
  if (web_frame_->Client())
    web_frame_->Client()->DidChangeLoadProgress(progress_estimate);
}
