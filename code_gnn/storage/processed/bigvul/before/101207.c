void ProcessCommitResponseCommand::ProcessCommitResponse(
    SyncSession* session) {
  ScopedDirLookup dir(session->context()->directory_manager(),
                      session->context()->account_name());
  if (!dir.good()) {
    LOG(ERROR) << "Scoped dir lookup failed!";
    return;
  }

  StatusController* status = session->status_controller();
  const ClientToServerResponse& response(status->commit_response());
  const CommitResponse& cr = response.commit();
  const sync_pb::CommitMessage& commit_message =
      status->commit_message().commit();

  int transient_error_commits = 0;
  int conflicting_commits = 0;
  int error_commits = 0;
  int successes = 0;
  set<syncable::Id> conflicting_new_folder_ids;
  set<syncable::Id> deleted_folders;
  ConflictProgress* conflict_progress = status->mutable_conflict_progress();
  OrderedCommitSet::Projection proj = status->commit_id_projection();
  if (!proj.empty()) {  
    WriteTransaction trans(FROM_HERE, SYNCER, dir);
    for (size_t i = 0; i < proj.size(); i++) {
      CommitResponse::ResponseType response_type =
          ProcessSingleCommitResponse(&trans, cr.entryresponse(proj[i]),
                                      commit_message.entries(proj[i]),
                                      status->GetCommitIdAt(proj[i]),
                                      &conflicting_new_folder_ids,
                                      &deleted_folders);
      switch (response_type) {
        case CommitResponse::INVALID_MESSAGE:
          ++error_commits;
          break;
        case CommitResponse::CONFLICT:
          ++conflicting_commits;
          conflict_progress->AddConflictingItemById(
              status->GetCommitIdAt(proj[i]));
          break;
        case CommitResponse::SUCCESS:
          ++successes;
          if (status->GetCommitIdModelTypeAt(proj[i]) == syncable::BOOKMARKS)
            status->increment_num_successful_bookmark_commits();
          status->increment_num_successful_commits();
          break;
        case CommitResponse::OVER_QUOTA:
        case CommitResponse::RETRY:
        case CommitResponse::TRANSIENT_ERROR:
          ++transient_error_commits;
          break;
        default:
          LOG(FATAL) << "Bad return from ProcessSingleCommitResponse";
      }
    }
  }

  status->increment_num_conflicting_commits_by(conflicting_commits);
  if (0 == successes) {
    status->increment_num_consecutive_transient_error_commits_by(
        transient_error_commits);
    status->increment_num_consecutive_errors_by(transient_error_commits);
  } else {
    status->set_num_consecutive_transient_error_commits(0);
    status->set_num_consecutive_errors(0);
  }
  int commit_count = static_cast<int>(proj.size());
  if (commit_count != (conflicting_commits + error_commits +
                       transient_error_commits)) {
    ResetErrorCounters(status);
  }
  SyncerUtil::MarkDeletedChildrenSynced(dir, &deleted_folders);

  return;
}
