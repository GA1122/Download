  void SyncRepeatedlyToTriggerStuckSignal(SyncSession* session) {
    for (int i = 0 ; i < 12 ; ++i)
      syncer_->SyncShare(session, SYNCER_BEGIN, SYNCER_END);
  }
