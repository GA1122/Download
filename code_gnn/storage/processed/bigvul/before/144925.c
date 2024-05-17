void RenderWidgetHostViewAura::Hide() {
  window_->Hide();

  if (host_ && !host_->is_hidden()) {
    host_->WasHidden();
    if (delegated_frame_host_)
      delegated_frame_host_->WasHidden();

#if defined(OS_WIN)
    aura::WindowTreeHost* host = window_->GetHost();
    if (host) {
      if (legacy_render_widget_host_HWND_)
        legacy_render_widget_host_HWND_->UpdateParent(ui::GetHiddenWindow());
    }
#endif
  }

#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_)
    legacy_render_widget_host_HWND_->Hide();
#endif
}
