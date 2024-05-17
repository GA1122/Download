SessionCommand* SessionService::CreateSetWindowBoundsCommand(
    const SessionID& window_id,
    const gfx::Rect& bounds,
    ui::WindowShowState show_state) {
  WindowBoundsPayload3 payload = { 0 };
  payload.window_id = window_id.id();
  payload.x = bounds.x();
  payload.y = bounds.y();
  payload.w = bounds.width();
  payload.h = bounds.height();
  payload.show_state = AdjustShowState(show_state);
  SessionCommand* command = new SessionCommand(kCommandSetWindowBounds3,
                                               sizeof(payload));
  memcpy(command->contents(), &payload, sizeof(payload));
  return command;
}
