  static TimeDelta GetNudgeDelayTimeDeltaFromType(
      const NudgeDelayStrategy& delay_type, const ModelType& model_type,
      const SyncManager::SyncInternal* core) {
    CHECK(core);
    TimeDelta delay = TimeDelta::FromMilliseconds(
       SyncManager::kDefaultNudgeDelayMilliseconds);
    switch (delay_type) {
     case IMMEDIATE:
       delay = TimeDelta::FromMilliseconds(
           SyncManager::kDefaultNudgeDelayMilliseconds);
       break;
     case ACCOMPANY_ONLY:
       delay = TimeDelta::FromSeconds(
           browser_sync::kDefaultShortPollIntervalSeconds);
       break;
     case CUSTOM:
       switch (model_type) {
         case syncable::PREFERENCES:
           delay = TimeDelta::FromMilliseconds(
               SyncManager::kPreferencesNudgeDelayMilliseconds);
           break;
         case syncable::SESSIONS:
           delay = core->scheduler()->sessions_commit_delay();
           break;
         default:
           NOTREACHED();
       }
       break;
     default:
       NOTREACHED();
    }
    return delay;
  }
