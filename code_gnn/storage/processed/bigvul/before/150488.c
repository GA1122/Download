void NavigationControllerImpl::SetNeedsReload() {
  needs_reload_ = true;

  if (last_committed_entry_index_ != -1) {
    entries_[last_committed_entry_index_]->SetTransitionType(
        ui::PAGE_TRANSITION_RELOAD);
  }
}
