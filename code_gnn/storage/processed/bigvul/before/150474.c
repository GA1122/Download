void NavigationControllerImpl::NavigateToPendingEntry(ReloadType reload_type) {
  needs_reload_ = false;

  if (pending_entry_index_ != -1 &&
      pending_entry_index_ == last_committed_entry_index_ &&
      (entries_[pending_entry_index_]->restore_type() == RestoreType::NONE) &&
      (entries_[pending_entry_index_]->GetTransitionType() &
       ui::PAGE_TRANSITION_FORWARD_BACK)) {
    delegate_->Stop();

    if (delegate_->GetInterstitialPage())
      delegate_->GetInterstitialPage()->DontProceed();

    DiscardNonCommittedEntries();
    return;
  }

  if (delegate_->GetInterstitialPage()) {
    static_cast<InterstitialPageImpl*>(delegate_->GetInterstitialPage())
        ->CancelForNavigation();
  }

  NavigationEntryImpl* last_navigation =
      last_pending_entry_ ? last_pending_entry_ : GetLastCommittedEntry();

  if (reload_type == ReloadType::NONE && last_navigation && pending_entry_ &&
      ShouldTreatNavigationAsReload(pending_entry_) &&
      !last_navigation->ssl_error() &&
      last_transient_entry_index_ == -1 &&
      pending_entry_->frame_tree_node_id() == -1 &&
      pending_entry_->GetURL() == last_navigation->GetURL() &&
      !pending_entry_->GetHasPostData() && !last_navigation->GetHasPostData() &&
      last_navigation->GetVirtualURL() == pending_entry_->GetVirtualURL() &&
      (pending_entry_->GetURL().SchemeIs(url::kDataScheme) &&
               pending_entry_->GetBaseURLForDataURL().is_valid()
           ? pending_entry_->GetBaseURLForDataURL() ==
                 last_navigation->GetBaseURLForDataURL()
           : true)) {
    reload_type = ReloadType::NORMAL;
  }

  if (last_pending_entry_index_ == -1 && last_pending_entry_)
    delete last_pending_entry_;

  last_transient_entry_index_ = -1;
  last_pending_entry_ = nullptr;
  last_pending_entry_index_ = -1;

  if (!pending_entry_) {
    CHECK_NE(pending_entry_index_, -1);
    pending_entry_ = entries_[pending_entry_index_].get();
  }

  if (IsRendererDebugURL(pending_entry_->GetURL())) {
    if (!delegate_->GetRenderViewHost()->IsRenderViewLive() &&
        !IsInitialNavigation()) {
      DiscardNonCommittedEntries();
      return;
    }
  }

  CHECK(!in_navigate_to_pending_entry_);
  in_navigate_to_pending_entry_ = true;
  bool success = NavigateToPendingEntryInternal(reload_type);
  in_navigate_to_pending_entry_ = false;

  if (!success)
    DiscardNonCommittedEntries();
}
