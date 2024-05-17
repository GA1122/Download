bool NavigationControllerImpl::CanPruneAllButVisible() {
  if (last_committed_entry_index_ == -1)
    return false;

  if (pending_entry_index_ != -1)
    return false;

  if (transient_entry_index_ != -1)
    return false;

  return true;
}
