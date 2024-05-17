void RenderWidgetHostViewAura::Show() {
  window_->Show();

  if (!host_->is_hidden())
    return;

  bool has_saved_frame =
      delegated_frame_host_ ? delegated_frame_host_->HasSavedFrame() : false;
  ui::LatencyInfo renderer_latency_info, browser_latency_info;
  if (has_saved_frame) {
    browser_latency_info.AddLatencyNumber(
        ui::TAB_SHOW_COMPONENT, host_->GetLatencyComponentId(), 0);
  } else {
    renderer_latency_info.AddLatencyNumber(
        ui::TAB_SHOW_COMPONENT, host_->GetLatencyComponentId(), 0);
  }
  host_->WasShown(renderer_latency_info);

  aura::Window* root = window_->GetRootWindow();
  if (root) {
    aura::client::CursorClient* cursor_client =
        aura::client::GetCursorClient(root);
    if (cursor_client)
      NotifyRendererOfCursorVisibilityState(cursor_client->IsCursorVisible());
  }

  if (delegated_frame_host_)
    delegated_frame_host_->WasShown(browser_latency_info);

#if defined(OS_WIN)
  UpdateLegacyWin();
#endif
}
