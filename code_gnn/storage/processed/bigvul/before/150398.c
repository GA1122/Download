void ClientControlledShellSurface::SetPip() {
  TRACE_EVENT0("exo", "ClientControlledShellSurface::SetPip");
  pending_window_state_ = ash::WindowStateType::kPip;
}
