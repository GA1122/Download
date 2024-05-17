  bool SyncShareAsDelegate() {
    scoped_ptr<SyncSession> session(MakeSession());
    syncer_->SyncShare(session.get(), SYNCER_BEGIN, SYNCER_END);
    return session->HasMoreToSync();
  }
