SessionCommand* SessionService::CreateSetWindowTypeCommand(
    const SessionID& window_id,
    WindowType type) {
  WindowTypePayload payload = { 0 };
  payload.id = window_id.id();
  payload.index = static_cast<int32>(type);
  SessionCommand* command = new SessionCommand(
      kCommandSetWindowType, sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
