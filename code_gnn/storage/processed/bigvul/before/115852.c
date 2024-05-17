void NavigationController::ReloadInternal(bool check_for_repost,
                                          ReloadType reload_type) {
  if (transient_entry_index_ != -1)
    return;

  DiscardNonCommittedEntriesInternal();
  int current_index = GetCurrentEntryIndex();
  if (current_index == -1) {
    return;
  }

  if (check_for_repost_ && check_for_repost &&
      GetEntryAtIndex(current_index)->has_post_data()) {
    content::NotificationService::current()->Notify(
        content::NOTIFICATION_REPOST_WARNING_SHOWN,
        content::Source<NavigationController>(this),
        content::NotificationService::NoDetails());

    pending_reload_ = reload_type;
    tab_contents_->Activate();
    tab_contents_->GetDelegate()->ShowRepostFormWarningDialog(tab_contents_);
  } else {
    DiscardNonCommittedEntriesInternal();

    pending_entry_index_ = current_index;
    entries_[pending_entry_index_]->set_transition_type(
        content::PAGE_TRANSITION_RELOAD);
    NavigateToPendingEntry(reload_type);
  }
}
