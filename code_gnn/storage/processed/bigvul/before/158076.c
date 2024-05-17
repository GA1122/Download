void LocalFrameClientImpl::DispatchDidReceiveResponse(
    const ResourceResponse& response) {
  if (web_frame_->Client()) {
    WrappedResourceResponse webresp(response);
    web_frame_->Client()->DidReceiveResponse(webresp);
  }
}
