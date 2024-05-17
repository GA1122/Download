void ProcessCommitResponseCommand::ProcessSuccessfulCommitResponse(
    const sync_pb::SyncEntity& committed_entry,
    const CommitResponse_EntryResponse& entry_response,
    const syncable::Id& pre_commit_id, syncable::MutableEntry* local_entry,
    bool syncing_was_set, set<syncable::Id>* deleted_folders) {
  DCHECK(local_entry->Get(IS_UNSYNCED));

  if (!UpdateVersionAfterCommit(committed_entry, entry_response, pre_commit_id,
                                local_entry)) {
    LOG(ERROR) << "Bad version in commit return for " << *local_entry
               << " new_id:" << entry_response.id() << " new_version:"
               << entry_response.version();
    return;
  }

  if (!ChangeIdAfterCommit(entry_response, pre_commit_id, local_entry)) {
    return;
  }

  UpdateServerFieldsAfterCommit(committed_entry, entry_response, local_entry);

  if (syncing_was_set) {
    OverrideClientFieldsAfterCommit(committed_entry, entry_response,
                                    local_entry);
    local_entry->Put(IS_UNSYNCED, false);
  }

  if (local_entry->Get(IS_DIR) && local_entry->Get(IS_DEL)) {
    deleted_folders->insert(local_entry->Get(ID));
  }
}
