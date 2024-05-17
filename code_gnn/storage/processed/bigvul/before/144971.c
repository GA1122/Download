void RenderWidgetHostViewAura::RemovingFromRootWindow() {
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(window_->GetRootWindow());
  if (cursor_client)
    cursor_client->RemoveObserver(this);

  DetachFromInputMethod();

  window_->GetHost()->RemoveObserver(this);
  if (delegated_frame_host_)
    delegated_frame_host_->ResetCompositor();

#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_)
    legacy_render_widget_host_HWND_->UpdateParent(ui::GetHiddenWindow());
#endif
}
