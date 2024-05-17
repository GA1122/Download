SessionCommand* SessionService::CreateSetSelectedTabInWindow(
    const SessionID& window_id,
    int index) {
  SelectedTabInIndexPayload payload = { 0 };
  payload.id = window_id.id();
  payload.index = index;
  SessionCommand* command = new SessionCommand(kCommandSetSelectedTabInIndex,
                                 sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
