ash::NonClientFrameViewAsh* ClientControlledShellSurface::GetFrameView() {
  return static_cast<ash::NonClientFrameViewAsh*>(
      widget_->non_client_view()->frame_view());
}
