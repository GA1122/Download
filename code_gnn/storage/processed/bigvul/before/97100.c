void NavigationController::CopyStateFrom(const NavigationController& source) {
  DCHECK(entry_count() == 0 && !pending_entry());

  if (source.entry_count() == 0)
    return;   

  needs_reload_ = true;
  for (int i = 0; i < source.entry_count(); i++) {
    entries_.push_back(linked_ptr<NavigationEntry>(
        new NavigationEntry(*source.entries_[i])));
  }

  FinishRestore(source.last_committed_entry_index_);
}
