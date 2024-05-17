void Syncer::SyncShare(sessions::SyncSession* session,
                       SyncerStep first_step,
                       SyncerStep last_step) {
  {
    ScopedDirLookup dir(session->context()->directory_manager(),
                        session->context()->account_name());
    CHECK(dir.good());
  }

  ScopedSessionContextConflictResolver scoped(session->context(),
                                              &resolver_);

  ScopedSyncStartStopTracker start_stop_tracker(session);
  SyncerStep current_step = first_step;

  SyncerStep next_step = current_step;
  while (!ExitRequested()) {
    switch (current_step) {
      case SYNCER_BEGIN:
        VLOG(1) << "Syncer Begin";
        session->context()->extensions_monitor()->GetAndClearRecords(
            session->mutable_extensions_activity());
        next_step = CLEANUP_DISABLED_TYPES;
        break;
      case CLEANUP_DISABLED_TYPES: {
        VLOG(1) << "Cleaning up disabled types";
        CleanupDisabledTypesCommand cleanup;
        cleanup.Execute(session);
        next_step = DOWNLOAD_UPDATES;
        break;
      }
      case DOWNLOAD_UPDATES: {
        VLOG(1) << "Downloading Updates";
        DownloadUpdatesCommand download_updates;
        download_updates.Execute(session);
        next_step = PROCESS_CLIENT_COMMAND;
        break;
      }
      case PROCESS_CLIENT_COMMAND: {
        VLOG(1) << "Processing Client Command";
        ProcessClientCommand(session);
        next_step = VERIFY_UPDATES;
        break;
      }
      case VERIFY_UPDATES: {
        VLOG(1) << "Verifying Updates";
        VerifyUpdatesCommand verify_updates;
        verify_updates.Execute(session);
        next_step = PROCESS_UPDATES;
        break;
      }
      case PROCESS_UPDATES: {
        VLOG(1) << "Processing Updates";
        ProcessUpdatesCommand process_updates;
        process_updates.Execute(session);
        next_step = STORE_TIMESTAMPS;
        break;
      }
      case STORE_TIMESTAMPS: {
        VLOG(1) << "Storing timestamps";
        StoreTimestampsCommand store_timestamps;
        store_timestamps.Execute(session);
        if (session->status_controller()->ServerSaysNothingMoreToDownload() ||
            !session->status_controller()->download_updates_succeeded()) {
          next_step = APPLY_UPDATES;
        } else {
          next_step = DOWNLOAD_UPDATES;
        }
        break;
      }
      case APPLY_UPDATES: {
        VLOG(1) << "Applying Updates";
        ApplyUpdatesCommand apply_updates;
        apply_updates.Execute(session);
        if (last_step == APPLY_UPDATES) {
          last_step = SYNCER_END;
          next_step = SYNCER_END;
        } else {
          next_step = BUILD_COMMIT_REQUEST;
        }
        break;
      }
      case BUILD_COMMIT_REQUEST: {
        VLOG(1) << "Processing Commit Request";
        ScopedDirLookup dir(session->context()->directory_manager(),
                            session->context()->account_name());
        if (!dir.good()) {
          LOG(ERROR) << "Scoped dir lookup failed!";
          return;
        }
        WriteTransaction trans(FROM_HERE, SYNCER, dir);
        sessions::ScopedSetSessionWriteTransaction set_trans(session, &trans);

        VLOG(1) << "Getting the Commit IDs";
        GetCommitIdsCommand get_commit_ids_command(
            session->context()->max_commit_batch_size());
        get_commit_ids_command.Execute(session);

        if (!session->status_controller()->commit_ids().empty()) {
          VLOG(1) << "Building a commit message";
          BuildCommitCommand build_commit_command;
          build_commit_command.Execute(session);

          next_step = POST_COMMIT_MESSAGE;
        } else {
          next_step = BUILD_AND_PROCESS_CONFLICT_SETS;
        }

        break;
      }
      case POST_COMMIT_MESSAGE: {
        VLOG(1) << "Posting a commit request";
        PostCommitMessageCommand post_commit_command;
        post_commit_command.Execute(session);
        next_step = PROCESS_COMMIT_RESPONSE;
        break;
      }
      case PROCESS_COMMIT_RESPONSE: {
        VLOG(1) << "Processing the commit response";
        session->status_controller()->reset_num_conflicting_commits();
        ProcessCommitResponseCommand process_response_command;
        process_response_command.Execute(session);
        next_step = BUILD_AND_PROCESS_CONFLICT_SETS;
        break;
      }
      case BUILD_AND_PROCESS_CONFLICT_SETS: {
        VLOG(1) << "Building and Processing Conflict Sets";
        BuildAndProcessConflictSetsCommand build_process_conflict_sets;
        build_process_conflict_sets.Execute(session);
        if (session->status_controller()->conflict_sets_built())
          next_step = SYNCER_END;
        else
          next_step = RESOLVE_CONFLICTS;
        break;
      }
      case RESOLVE_CONFLICTS: {
        VLOG(1) << "Resolving Conflicts";

        if (pre_conflict_resolution_closure_) {
          pre_conflict_resolution_closure_->Run();
        }

        StatusController* status = session->status_controller();
        status->reset_conflicts_resolved();
        ResolveConflictsCommand resolve_conflicts_command;
        resolve_conflicts_command.Execute(session);

        if (status->HasConflictingUpdates())
          next_step = APPLY_UPDATES_TO_RESOLVE_CONFLICTS;
        else
          next_step = SYNCER_END;
        break;
      }
      case APPLY_UPDATES_TO_RESOLVE_CONFLICTS: {
        StatusController* status = session->status_controller();
        VLOG(1) << "Applying updates to resolve conflicts";
        ApplyUpdatesCommand apply_updates;

        int before_blocking_conflicting_updates =
            status->TotalNumBlockingConflictingItems();
        apply_updates.Execute(session);
        int after_blocking_conflicting_updates =
            status->TotalNumBlockingConflictingItems();
        status->update_conflicts_resolved(before_blocking_conflicting_updates >
                                          after_blocking_conflicting_updates);
        if (status->conflicts_resolved())
          next_step = RESOLVE_CONFLICTS;
        else
          next_step = SYNCER_END;
        break;
      }
      case CLEAR_PRIVATE_DATA: {
        VLOG(1) << "Clear Private Data";
        ClearDataCommand clear_data_command;
        clear_data_command.Execute(session);
        next_step = SYNCER_END;
        break;
      }
      case SYNCER_END: {
        VLOG(1) << "Syncer End";
        SyncerEndCommand syncer_end_command;
        syncer_end_command.Execute(session);
        next_step = SYNCER_END;
        break;
      }
      default:
        LOG(ERROR) << "Unknown command: " << current_step;
    }
    VLOG(2) << "last step: " << last_step << ", current step: "
            << current_step << ", next step: "
            << next_step << ", snapshot: "
            << session->TakeSnapshot().ToString();
    if (last_step == current_step)
      break;
    current_step = next_step;
  }
}
