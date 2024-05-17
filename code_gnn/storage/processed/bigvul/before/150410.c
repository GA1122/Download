void ClientControlledShellSurface::UpdateAutoHideFrame() {
  if (immersive_fullscreen_controller_) {
    bool enabled = (frame_type_ == SurfaceFrameType::AUTOHIDE &&
                    (GetWindowState()->IsMaximizedOrFullscreenOrPinned() ||
                     GetWindowState()->IsSnapped()));
    ash::ImmersiveFullscreenController::EnableForWidget(widget_, enabled);
  }
}
