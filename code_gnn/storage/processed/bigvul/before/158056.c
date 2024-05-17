void LocalFrameClientImpl::DidObserveLoadingBehavior(
    WebLoadingBehaviorFlag behavior) {
  if (web_frame_->Client())
    web_frame_->Client()->DidObserveLoadingBehavior(behavior);
}
