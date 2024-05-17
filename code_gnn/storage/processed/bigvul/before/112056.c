  bool SyncShareNudge() {
    session_.reset(MakeSession());
    return SyncShareAsDelegate(SyncScheduler::SyncSessionJob::NUDGE);
  }
