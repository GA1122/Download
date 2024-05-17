void LoginDisplayHostWebUI::OnDisplayMetricsChanged(
    const display::Display& display,
    uint32_t changed_metrics) {
  const display::Display primary_display =
      display::Screen::GetScreen()->GetPrimaryDisplay();
  if (display.id() != primary_display.id() ||
      !(changed_metrics & DISPLAY_METRIC_BOUNDS)) {
    return;
  }

  if (GetOobeUI()) {
    if (is_voice_interaction_oobe_)
      login_window_->SetSize(primary_display.work_area_size());

    const gfx::Size& size = primary_display.size();
    GetOobeUI()->GetCoreOobeView()->SetClientAreaSize(size.width(),
                                                      size.height());

    if (changed_metrics & DISPLAY_METRIC_PRIMARY)
      GetOobeUI()->OnDisplayConfigurationChanged();
  }
}
