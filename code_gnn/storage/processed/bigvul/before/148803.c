void InterstitialPageImpl::Proceed() {
  if (!render_view_host_)
    return;

  if (action_taken_ != NO_ACTION) {
    NOTREACHED();
    return;
  }
  Disable();
  action_taken_ = PROCEED_ACTION;

  pause_throbber_ = false;
  controller_->delegate()->DidProceedOnInterstitial();

  if (new_navigation_)
    TakeActionOnResourceDispatcher(CANCEL);
  else
    TakeActionOnResourceDispatcher(RESUME);

  if (!new_navigation_) {
    Hide();
    delegate_->OnProceed();
    return;
  }

  delegate_->OnProceed();
}
