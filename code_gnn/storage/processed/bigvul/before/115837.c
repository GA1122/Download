void NavigationController::AddTransientEntry(NavigationEntry* entry) {
  int index = 0;
  if (last_committed_entry_index_ != -1)
    index = last_committed_entry_index_ + 1;
  DiscardTransientEntry();
  entries_.insert(entries_.begin() + index, linked_ptr<NavigationEntry>(entry));
  transient_entry_index_ = index;
  tab_contents_->NotifyNavigationStateChanged(kInvalidateAll);
}
