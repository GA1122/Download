void RenderWidgetHostViewAura::OnLegacyWindowDestroyed() {
  legacy_render_widget_host_HWND_ = NULL;
  legacy_window_destroyed_ = true;
}
