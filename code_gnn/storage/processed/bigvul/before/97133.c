void NavigationController::Reload(bool check_for_repost) {
  if (transient_entry_index_ != -1)
    return;

  DiscardNonCommittedEntriesInternal();
  int current_index = GetCurrentEntryIndex();
  if (check_for_repost_ && check_for_repost && current_index != -1 &&
      GetEntryAtIndex(current_index)->has_post_data()) {
    tab_contents_->Activate();
    tab_contents_->delegate()->ShowRepostFormWarningDialog(tab_contents_);
  } else {
    int current_index = GetCurrentEntryIndex();

    if (current_index == -1)
      return;

    DiscardNonCommittedEntriesInternal();

    pending_entry_index_ = current_index;
    entries_[pending_entry_index_]->set_transition_type(PageTransition::RELOAD);
    NavigateToPendingEntry(true);
  }
}
