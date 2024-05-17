void NavigationControllerImpl::NavigateToPendingEntry(
    ReloadType reload_type,
    std::unique_ptr<NavigationUIData> navigation_ui_data) {
  DCHECK(pending_entry_);
  needs_reload_ = false;

  if (pending_entry_index_ != -1 &&
      pending_entry_index_ == last_committed_entry_index_ &&
      pending_entry_->restore_type() == RestoreType::NONE &&
      pending_entry_->GetTransitionType() & ui::PAGE_TRANSITION_FORWARD_BACK) {
    delegate_->Stop();

    InterstitialPage* interstitial =
        InterstitialPage::GetInterstitialPage(GetWebContents());
    if (interstitial)
      interstitial->DontProceed();

    DiscardNonCommittedEntries();
    return;
  }

  if (delegate_->GetInterstitialPage()) {
    static_cast<InterstitialPageImpl*>(delegate_->GetInterstitialPage())
        ->CancelForNavigation();
  }

  if (ShouldTreatNavigationAsReload(
          pending_entry_->GetURL(), pending_entry_->GetVirtualURL(),
          pending_entry_->GetBaseURLForDataURL(),
          pending_entry_->GetTransitionType(),
          pending_entry_->frame_tree_node_id() == -1  ,
          pending_entry_->GetHasPostData()  ,
          reload_type != ReloadType::NONE  ,
          pending_entry_index_ != -1  ,
          transient_entry_index_ != -1  ,
          GetLastCommittedEntry())) {
    reload_type = ReloadType::NORMAL;
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
  bool success = NavigateToPendingEntryInternal(reload_type,
                                                std::move(navigation_ui_data));
  in_navigate_to_pending_entry_ = false;

  if (!success)
    DiscardNonCommittedEntries();
}
