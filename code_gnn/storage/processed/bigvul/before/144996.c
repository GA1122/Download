void RenderWidgetHostViewAura::UpdateLegacyWin() {
  if (legacy_window_destroyed_ || !GetHostWindowHWND())
    return;

  if (!legacy_render_widget_host_HWND_) {
    legacy_render_widget_host_HWND_ =
        LegacyRenderWidgetHostHWND::Create(GetHostWindowHWND());
  }

  if (legacy_render_widget_host_HWND_) {
    legacy_render_widget_host_HWND_->set_host(this);
    legacy_render_widget_host_HWND_->UpdateParent(GetHostWindowHWND());
    legacy_render_widget_host_HWND_->SetBounds(
        window_->GetBoundsInRootWindow());
    if (!host_->is_hidden())
      legacy_render_widget_host_HWND_->Show();
  }
}
