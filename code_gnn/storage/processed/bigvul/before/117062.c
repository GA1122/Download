SessionCommand* SessionService::CreateSetTabWindowCommand(
    const SessionID& window_id,
    const SessionID& tab_id) {
  SessionID::id_type payload[] = { window_id.id(), tab_id.id() };
  SessionCommand* command =
      new SessionCommand(kCommandSetTabWindow, sizeof(payload));
  memcpy(command->contents(), payload, sizeof(payload));
  return command;
}
