bool NavigationControllerImpl::RemoveEntryAtIndex(int index) {
  if (index == last_committed_entry_index_ ||
      index == pending_entry_index_)
    return false;

  RemoveEntryAtIndexInternal(index);
  return true;
}
