SessionCommand* SessionService::CreateSetSelectedNavigationIndexCommand(
    const SessionID& tab_id,
    int index) {
  SelectedNavigationIndexPayload payload = { 0 };
  payload.id = tab_id.id();
  payload.index = index;
  SessionCommand* command = new SessionCommand(
      kCommandSetSelectedNavigationIndex, sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
