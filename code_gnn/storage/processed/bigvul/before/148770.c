void InterstitialPageImpl::FocusThroughTabTraversal(bool reverse) {
  if (!enabled())
    return;
  render_view_host_->SetInitialFocus(reverse);
}
