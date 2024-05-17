ProcessCommitResponseCommand::ProcessSingleCommitResponse(
    syncable::WriteTransaction* trans,
    const sync_pb::CommitResponse_EntryResponse& pb_server_entry,
    const sync_pb::SyncEntity& commit_request_entry,
    const syncable::Id& pre_commit_id,
    std::set<syncable::Id>* conflicting_new_folder_ids,
    set<syncable::Id>* deleted_folders) {

  const CommitResponse_EntryResponse& server_entry =
      *static_cast<const CommitResponse_EntryResponse*>(&pb_server_entry);
  MutableEntry local_entry(trans, GET_BY_ID, pre_commit_id);
  CHECK(local_entry.good());
  bool syncing_was_set = local_entry.Get(SYNCING);
  local_entry.Put(SYNCING, false);

  CommitResponse::ResponseType response = (CommitResponse::ResponseType)
      server_entry.response_type();
  if (!CommitResponse::ResponseType_IsValid(response)) {
    LOG(ERROR) << "Commit response has unknown response type! Possibly out "
               "of date client?";
    return CommitResponse::INVALID_MESSAGE;
  }
  if (CommitResponse::TRANSIENT_ERROR == response) {
    VLOG(1) << "Transient Error Committing: " << local_entry;
    LogServerError(server_entry);
    return CommitResponse::TRANSIENT_ERROR;
  }
  if (CommitResponse::INVALID_MESSAGE == response) {
    LOG(ERROR) << "Error Commiting: " << local_entry;
    LogServerError(server_entry);
    return response;
  }
  if (CommitResponse::CONFLICT == response) {
    VLOG(1) << "Conflict Committing: " << local_entry;
    if (!pre_commit_id.ServerKnows() && local_entry.Get(IS_DIR)) {
      conflicting_new_folder_ids->insert(pre_commit_id);
    }
    return response;
  }
  if (CommitResponse::RETRY == response) {
    VLOG(1) << "Retry Committing: " << local_entry;
    return response;
  }
  if (CommitResponse::OVER_QUOTA == response) {
    LOG(WARNING) << "Hit deprecated OVER_QUOTA Committing: " << local_entry;
    return response;
  }
  if (!server_entry.has_id_string()) {
    LOG(ERROR) << "Commit response has no id";
    return CommitResponse::INVALID_MESSAGE;
  }

  DCHECK_EQ(CommitResponse::SUCCESS, response) << response;
  if (pre_commit_id != server_entry.id()) {
    Entry e(trans, GET_BY_ID, server_entry.id());
    if (e.good()) {
      LOG(ERROR) << "Got duplicate id when commiting id: " << pre_commit_id <<
                 ". Treating as an error return";
      return CommitResponse::INVALID_MESSAGE;
    }
  }

  if (server_entry.version() == 0) {
    LOG(WARNING) << "Server returned a zero version on a commit response.";
  }

  ProcessSuccessfulCommitResponse(commit_request_entry, server_entry,
      pre_commit_id, &local_entry, syncing_was_set, deleted_folders);
  return response;
}
