void ClientControlledShellSurface::SetSnappedToLeft() {
  TRACE_EVENT0("exo", "ClientControlledShellSurface::SetSnappedToLeft");
  pending_window_state_ = ash::WindowStateType::kLeftSnapped;
}
