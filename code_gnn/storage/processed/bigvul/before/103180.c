void Browser::SaveWindowPlacement(const gfx::Rect& bounds, bool maximized) {
  if (profile()->HasSessionService()) {
    SessionService* session_service = profile()->GetSessionService();
    if (session_service)
      session_service->SetWindowBounds(session_id_, bounds, maximized);
  }
}
