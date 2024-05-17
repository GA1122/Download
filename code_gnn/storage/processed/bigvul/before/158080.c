void LocalFrameClientImpl::DispatchWillSendRequest(ResourceRequest& request) {
  if (web_frame_->Client()) {
    WrappedResourceRequest webreq(request);
    web_frame_->Client()->WillSendRequest(webreq);
  }
}
