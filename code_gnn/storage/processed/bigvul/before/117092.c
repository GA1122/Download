void SessionService::SetPinnedState(const SessionID& window_id,
                                    const SessionID& tab_id,
                                    bool is_pinned) {
  if (!ShouldTrackChangesToWindow(window_id))
    return;

  ScheduleCommand(CreatePinnedStateCommand(tab_id, is_pinned));
}
