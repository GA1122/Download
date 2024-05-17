void RenderWidgetHostViewAura::OnDisplayMetricsChanged(
    const display::Display& display,
    uint32_t metrics) {
  display::Screen* screen = display::Screen::GetScreen();
  if (display.id() == screen->GetDisplayNearestWindow(window_).id()) {
    UpdateScreenInfo(window_);
    current_cursor_.SetDisplayInfo(display);
    UpdateCursorIfOverSelf();
  }
}
