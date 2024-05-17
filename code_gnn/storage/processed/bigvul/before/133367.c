void WindowTreeHostManager::OnDisplayRemoved(const display::Display& display) {
  AshWindowTreeHost* host_to_delete = window_tree_hosts_[display.id()];
  CHECK(host_to_delete) << display.ToString();

  if (primary_display_id == display.id()) {
    if (window_tree_hosts_.size() == 1) {
      primary_display_id = display::Display::kInvalidDisplayID;
      primary_tree_host_for_replace_ = host_to_delete;
      window_tree_hosts_.erase(display.id());
      return;
    }
    for (const auto& pair : window_tree_hosts_) {
      if (pair.first != display.id()) {
        primary_display_id = pair.first;
        break;
      }
    }
    CHECK_NE(display::Display::kInvalidDisplayID, primary_display_id);

    AshWindowTreeHost* primary_host = host_to_delete;
    host_to_delete = window_tree_hosts_[primary_display_id];
    GetRootWindowSettings(GetWindow(host_to_delete))->display_id = display.id();

    window_tree_hosts_[primary_display_id] = primary_host;
    GetRootWindowSettings(GetWindow(primary_host))->display_id =
        primary_display_id;

    OnDisplayMetricsChanged(
        GetDisplayManager()->GetDisplayForId(primary_display_id),
        DISPLAY_METRIC_BOUNDS);
  }

  DeleteHost(host_to_delete);

  window_tree_hosts_.erase(display.id());
}
