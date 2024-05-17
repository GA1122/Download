void LocalFrameClientImpl::DispatchDidFailProvisionalLoad(
    const ResourceError& error,
    WebHistoryCommitType commit_type) {
  web_frame_->DidFail(error, true, commit_type);
  virtual_time_pauser_.UnpauseVirtualTime();
}
