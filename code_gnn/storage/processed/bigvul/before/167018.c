void NavigationControllerImpl::LoadIfNecessary() {
  if (!needs_reload_)
    return;

  if (pending_entry_) {
    NavigateToPendingEntry(ReloadType::NONE, nullptr  );
  } else if (last_committed_entry_index_ != -1) {
    pending_entry_ = entries_[last_committed_entry_index_].get();
    pending_entry_index_ = last_committed_entry_index_;
    NavigateToPendingEntry(ReloadType::NONE, nullptr  );
  } else {
    needs_reload_ = false;
  }
}
