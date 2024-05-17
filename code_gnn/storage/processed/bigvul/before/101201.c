bool ProcessCommitResponseCommand::ChangeIdAfterCommit(
    const CommitResponse_EntryResponse& entry_response,
    const syncable::Id& pre_commit_id,
    syncable::MutableEntry* local_entry) {
  syncable::WriteTransaction* trans = local_entry->write_transaction();
  if (entry_response.id() != pre_commit_id) {
    if (pre_commit_id.ServerKnows()) {
      VLOG(1) << " ID changed while committing an old entry. "
              << pre_commit_id << " became " << entry_response.id() << ".";
    }
    MutableEntry same_id(trans, GET_BY_ID, entry_response.id());
    if (same_id.good()) {
      LOG(ERROR) << "ID clash with id " << entry_response.id()
                 << " during commit " << same_id;
      return false;
    }
    SyncerUtil::ChangeEntryIDAndUpdateChildren(
        trans, local_entry, entry_response.id());
    VLOG(1) << "Changing ID to " << entry_response.id();
  }
  return true;
}
