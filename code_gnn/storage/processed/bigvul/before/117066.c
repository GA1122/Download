SessionCommand* SessionService::CreateWindowClosedCommand(
    const SessionID::id_type window_id) {
  ClosedPayload payload;
  memset(&payload, 0, sizeof(payload));
  payload.id = window_id;
  payload.close_time = Time::Now().ToInternalValue();
  SessionCommand* command =
      new SessionCommand(kCommandWindowClosed, sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
