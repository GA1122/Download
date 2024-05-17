void BackgroundLoaderOffliner::DocumentOnLoadCompletedInMainFrame() {
  if (!pending_request_.get()) {
    DVLOG(1) << "DidStopLoading called even though no pending request.";
    return;
  }

  AddLoadingSignal("DocumentOnLoadCompletedInMainFrame");

  snapshot_controller_->DocumentOnLoadCompletedInMainFrame();
}
