void RenderWidgetHostImpl::ForceFirstFrameAfterNavigationTimeout() {
  if (did_receive_first_frame_after_navigation_ ||
      !new_content_rendering_timeout_) {
    return;
  }
  new_content_rendering_timeout_->Stop();
  ClearDisplayedGraphics();
}
