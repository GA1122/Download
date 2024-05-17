bool ProcessCommitResponseCommand::UpdateVersionAfterCommit(
    const sync_pb::SyncEntity& committed_entry,
    const CommitResponse_EntryResponse& entry_response,
    const syncable::Id& pre_commit_id,
    syncable::MutableEntry* local_entry) {
  int64 old_version = local_entry->Get(BASE_VERSION);
  int64 new_version = entry_response.version();
  bool bad_commit_version = false;
  if (committed_entry.deleted() &&
      !local_entry->Get(syncable::UNIQUE_CLIENT_TAG).empty()) {
    new_version = 0;
  } else if (!pre_commit_id.ServerKnows()) {
    bad_commit_version = 0 == new_version;
  } else {
    bad_commit_version = old_version > new_version;
  }
  if (bad_commit_version) {
    LOG(ERROR) << "Bad version in commit return for " << *local_entry
               << " new_id:" << entry_response.id() << " new_version:"
               << entry_response.version();
    return false;
  }

  local_entry->Put(BASE_VERSION, new_version);
  VLOG(1) << "Commit is changing base version of " << local_entry->Get(ID)
          << " to: " << new_version;
  local_entry->Put(SERVER_VERSION, new_version);
  return true;
}
