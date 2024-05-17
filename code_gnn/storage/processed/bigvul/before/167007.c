void NavigationControllerImpl::DeleteNavigationEntries(
    const DeletionPredicate& deletionPredicate) {
  CHECK(CanPruneAllButLastCommitted());
  std::vector<int> delete_indices;
  for (size_t i = 0; i < entries_.size(); i++) {
    if (i != static_cast<size_t>(last_committed_entry_index_) &&
        deletionPredicate.Run(*entries_[i])) {
      delete_indices.push_back(i);
    }
  }
  if (delete_indices.empty())
    return;

  if (delete_indices.size() == GetEntryCount() - 1U) {
    PruneAllButLastCommitted();
  } else {
    for (auto it = delete_indices.rbegin(); it != delete_indices.rend(); ++it) {
      RemoveEntryAtIndex(*it);
    }
    delegate_->SetHistoryOffsetAndLength(last_committed_entry_index_,
                                         GetEntryCount());
  }
  delegate()->NotifyNavigationEntriesDeleted();
}
