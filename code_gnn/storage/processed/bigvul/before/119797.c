void NavigationControllerImpl::NavigateToPendingEntry(ReloadType reload_type) {
  needs_reload_ = false;

  if (pending_entry_index_ != -1 &&
      pending_entry_index_ == last_committed_entry_index_ &&
      (entries_[pending_entry_index_]->restore_type() ==
          NavigationEntryImpl::RESTORE_NONE) &&
      (entries_[pending_entry_index_]->GetTransitionType() &
          PAGE_TRANSITION_FORWARD_BACK)) {
    web_contents_->Stop();

    if (web_contents_->GetInterstitialPage())
      web_contents_->GetInterstitialPage()->DontProceed();

    DiscardNonCommittedEntries();
    return;
  }

  if (web_contents_->GetInterstitialPage()) {
    static_cast<InterstitialPageImpl*>(web_contents_->GetInterstitialPage())->
        CancelForNavigation();
  }

  if (!pending_entry_) {
    DCHECK_NE(pending_entry_index_, -1);
    pending_entry_ = entries_[pending_entry_index_].get();
  }

  if (!web_contents_->NavigateToPendingEntry(reload_type))
    DiscardNonCommittedEntries();

  if (pending_entry_ && !pending_entry_->site_instance() &&
      pending_entry_->restore_type() != NavigationEntryImpl::RESTORE_NONE) {
    pending_entry_->set_site_instance(static_cast<SiteInstanceImpl*>(
        web_contents_->GetPendingSiteInstance()));
    pending_entry_->set_restore_type(NavigationEntryImpl::RESTORE_NONE);
  }
}
