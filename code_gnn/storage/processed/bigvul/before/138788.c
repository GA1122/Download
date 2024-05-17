void RenderFrameHostImpl::ResetLoadingState() {
  if (is_loading()) {
    if (!is_active())
      is_loading_ = false;
    else
      OnDidStopLoading();
  }
}
