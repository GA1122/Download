void SyncManager::SyncInternal::RequestNudgeForDataType(
    const tracked_objects::Location& nudge_location,
    const ModelType& type) {
  if (!scheduler()) {
    NOTREACHED();
    return;
  }
  base::TimeDelta nudge_delay;
  switch (type) {
    case syncable::PREFERENCES:
      nudge_delay =
          TimeDelta::FromMilliseconds(kPreferencesNudgeDelayMilliseconds);
      break;
    case syncable::SESSIONS:
      nudge_delay = scheduler()->sessions_commit_delay();
      break;
    default:
      nudge_delay =
          TimeDelta::FromMilliseconds(kDefaultNudgeDelayMilliseconds);
      break;
  }
  syncable::ModelTypeBitSet types;
  types.set(type);
  scheduler()->ScheduleNudge(nudge_delay,
                             browser_sync::NUDGE_SOURCE_LOCAL,
                             types,
                             nudge_location);
}
