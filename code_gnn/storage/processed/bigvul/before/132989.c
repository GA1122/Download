void RenderWidgetHostViewAura::OnDisplayBoundsChanged(
    const gfx::Display& display) {
  gfx::Screen* screen = gfx::Screen::GetScreenFor(window_);
  if (display.id() == screen->GetDisplayNearestWindow(window_).id()) {
    UpdateScreenInfo(window_);
    current_cursor_.SetDisplayInfo(display);
    UpdateCursorIfOverSelf();
  }
}
