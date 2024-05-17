void InterstitialPageImpl::DontProceed() {
  if (!render_view_host_)
    return;
  DCHECK(action_taken_ != DONT_PROCEED_ACTION);

  Disable();
  action_taken_ = DONT_PROCEED_ACTION;

  if (new_navigation_)
    TakeActionOnResourceDispatcher(RESUME);
  else
    TakeActionOnResourceDispatcher(CANCEL);

  if (should_discard_pending_nav_entry_) {
    controller_->DiscardNonCommittedEntries();
  }

  Hide();
  delegate_->OnDontProceed();
}
