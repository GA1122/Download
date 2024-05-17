void ClientControlledShellSurface::SetFullscreen(bool fullscreen) {
  TRACE_EVENT1("exo", "ClientControlledShellSurface::SetFullscreen",
               "fullscreen", fullscreen);
  pending_window_state_ = fullscreen ? ash::WindowStateType::kFullscreen
                                     : ash::WindowStateType::kNormal;
}
