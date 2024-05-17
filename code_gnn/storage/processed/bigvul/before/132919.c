gfx::Rect RenderWidgetHostViewAura::ConvertRectFromScreen(
    const gfx::Rect& rect) const {
  gfx::Point origin = rect.origin();
  gfx::Point end = gfx::Point(rect.right(), rect.bottom());

  aura::Window* root_window = window_->GetRootWindow();
  if (root_window) {
    aura::client::ScreenPositionClient* screen_position_client =
        aura::client::GetScreenPositionClient(root_window);
    screen_position_client->ConvertPointFromScreen(window_, &origin);
    screen_position_client->ConvertPointFromScreen(window_, &end);
    return gfx::Rect(origin.x(),
                     origin.y(),
                     end.x() - origin.x(),
                     end.y() - origin.y());
  }

  return rect;
}
