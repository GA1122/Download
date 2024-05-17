void SessionService::SetTabExtensionAppID(
    const SessionID& window_id,
    const SessionID& tab_id,
    const std::string& extension_app_id) {
  if (!ShouldTrackChangesToWindow(window_id))
    return;

  ScheduleCommand(CreateSetTabExtensionAppIDCommand(
                      kCommandSetExtensionAppID,
                      tab_id.id(),
                      extension_app_id));
}
