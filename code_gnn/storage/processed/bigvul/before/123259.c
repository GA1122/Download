void RenderWidgetHostViewAura::UpdateCursor(const WebCursor& cursor) {
  current_cursor_ = cursor;
  const gfx::Display display = gfx::Screen::GetScreenFor(window_)->
      GetDisplayNearestWindow(window_);
  current_cursor_.SetScaleFactor(display.device_scale_factor());
  UpdateCursorIfOverSelf();
}
