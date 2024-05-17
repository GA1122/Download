void LocalFrameClientImpl::DidObserveNewCssPropertyUsage(int css_property,
                                                         bool is_animated) {
  if (web_frame_->Client()) {
    web_frame_->Client()->DidObserveNewCssPropertyUsage(css_property,
                                                        is_animated);
  }
}
