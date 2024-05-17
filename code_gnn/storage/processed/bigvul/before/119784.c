void NavigationControllerImpl::GoBack() {
  if (!CanGoBack()) {
    NOTREACHED();
    return;
  }

  int current_index = GetCurrentEntryIndex();

  DiscardNonCommittedEntries();

  pending_entry_index_ = current_index - 1;
  entries_[pending_entry_index_]->SetTransitionType(
      PageTransitionFromInt(
          entries_[pending_entry_index_]->GetTransitionType() |
          PAGE_TRANSITION_FORWARD_BACK));
  NavigateToPendingEntry(NO_RELOAD);
}
