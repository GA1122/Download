void RenderWidgetHostViewAura::SetBounds(const gfx::Rect& rect) {
  gfx::Point relative_origin(rect.origin());

  aura::Window* root = window_->GetRootWindow();
  if (root) {
    aura::client::ScreenPositionClient* screen_position_client =
        aura::client::GetScreenPositionClient(root);
    if (screen_position_client) {
      screen_position_client->ConvertPointFromScreen(
          window_->parent(), &relative_origin);
    }
  }

  InternalSetBounds(gfx::Rect(relative_origin, rect.size()));
}
