void SyncManager::SyncInternal::RequestNudge(
    const tracked_objects::Location& location) {
  if (scheduler())
     scheduler()->ScheduleNudge(
        TimeDelta::FromMilliseconds(0), browser_sync::NUDGE_SOURCE_LOCAL,
        ModelTypeBitSet(), location);
}
