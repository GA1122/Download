void WindowTreeHostManager::OnDisplayMetricsChanged(
    const display::Display& display,
    uint32_t metrics) {
  if (!(metrics & (DISPLAY_METRIC_BOUNDS | DISPLAY_METRIC_ROTATION |
                   DISPLAY_METRIC_DEVICE_SCALE_FACTOR)))
    return;
  const DisplayInfo& display_info =
      GetDisplayManager()->GetDisplayInfo(display.id());
  DCHECK(!display_info.bounds_in_native().IsEmpty());
  AshWindowTreeHost* ash_host = window_tree_hosts_[display.id()];
  ash_host->AsWindowTreeHost()->SetBounds(display_info.bounds_in_native());
  SetDisplayPropertiesOnHost(ash_host, display);
}
