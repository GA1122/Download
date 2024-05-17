void SyncManager::SyncInternal::RequestNudgeForDataTypes(
    const tracked_objects::Location& nudge_location,
    ModelTypeSet types) {
  if (!scheduler()) {
    NOTREACHED();
    return;
  }

  debug_info_event_listener_.OnNudgeFromDatatype(types.First().Get());

  base::TimeDelta nudge_delay = NudgeStrategy::GetNudgeDelayTimeDelta(
      types.First().Get(),
      this);
  scheduler()->ScheduleNudge(nudge_delay,
                             browser_sync::NUDGE_SOURCE_LOCAL,
                             types,
                             nudge_location);
}
