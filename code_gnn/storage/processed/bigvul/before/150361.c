const ash::NonClientFrameViewAsh* ClientControlledShellSurface::GetFrameView()
    const {
  return static_cast<const ash::NonClientFrameViewAsh*>(
      widget_->non_client_view()->frame_view());
}
