void ClientControlledShellSurface::SetSnappedToRight() {
  TRACE_EVENT0("exo", "ClientControlledShellSurface::SetSnappedToRight");
  pending_window_state_ = ash::WindowStateType::kRightSnapped;
}
