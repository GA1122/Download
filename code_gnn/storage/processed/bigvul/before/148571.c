bool WebContentsImpl::ShouldTransferNavigation(bool is_main_frame_navigation) {
  if (!delegate_)
    return true;
  return delegate_->ShouldTransferNavigation(is_main_frame_navigation);
}
