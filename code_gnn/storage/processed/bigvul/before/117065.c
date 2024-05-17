SessionCommand* SessionService::CreateTabClosedCommand(
    const SessionID::id_type tab_id) {
  ClosedPayload payload;
  memset(&payload, 0, sizeof(payload));
  payload.id = tab_id;
  payload.close_time = Time::Now().ToInternalValue();
  SessionCommand* command =
      new SessionCommand(kCommandTabClosed, sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
