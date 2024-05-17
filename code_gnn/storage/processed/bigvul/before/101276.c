  void SyncRepeatedlyToTriggerConflictResolution(SyncSession* session) {
    for (int i = 0 ; i < 6 ; ++i)
      syncer_->SyncShare(session, SYNCER_BEGIN, SYNCER_END);
  }
