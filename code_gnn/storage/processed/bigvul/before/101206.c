void ProcessCommitResponseCommand::OverrideClientFieldsAfterCommit(
    const sync_pb::SyncEntity& committed_entry,
    const CommitResponse_EntryResponse& entry_response,
    syncable::MutableEntry* local_entry) {
  if (committed_entry.deleted()) {
    DCHECK(local_entry->Get(IS_DEL));
    return;
  }

  const string& server_name =
      GetResultingPostCommitName(committed_entry, entry_response);
  const string& old_name =
      local_entry->Get(syncable::NON_UNIQUE_NAME);

  if (!server_name.empty() && old_name != server_name) {
    VLOG(1) << "During commit, server changed name: " << old_name
            << " to new name: " << server_name;
    local_entry->Put(syncable::NON_UNIQUE_NAME, server_name);
  }

  if (entry_response.has_position_in_parent()) {
    DCHECK_EQ(entry_response.position_in_parent(),
        local_entry->Get(SERVER_POSITION_IN_PARENT));

    syncable::Id new_prev = local_entry->ComputePrevIdFromServerPosition(
        local_entry->Get(PARENT_ID));
    if (!local_entry->PutPredecessor(new_prev))
      LOG(WARNING) << "PutPredecessor failed after successful commit";
  }
}
