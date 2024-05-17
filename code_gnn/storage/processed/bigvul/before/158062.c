void LocalFrameClientImpl::DidStartLoading(LoadStartType load_start_type) {
  if (web_frame_->Client()) {
    web_frame_->Client()->DidStartLoading(load_start_type ==
                                          kNavigationToDifferentDocument);
  }
}
