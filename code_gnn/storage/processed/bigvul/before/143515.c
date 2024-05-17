void CompositorImpl::OnDisplayMetricsChanged(const display::Display& display,
                                             uint32_t changed_metrics) {
  if (changed_metrics & display::DisplayObserver::DisplayMetric::
                            DISPLAY_METRIC_DEVICE_SCALE_FACTOR &&
      display.id() == display::Screen::GetScreen()
                          ->GetDisplayNearestWindow(root_window_)
                          .id()) {
    host_->SetViewportSizeAndScale(size_, root_window_->GetDipScale(),
                                   GenerateLocalSurfaceId());
  }
}
