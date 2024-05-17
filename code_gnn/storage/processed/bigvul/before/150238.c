WindowStateType TabletModeWindowState::GetMaximizedOrCenteredWindowType(
    wm::WindowState* window_state) {
  return (window_state->CanMaximize() &&
          ::wm::GetTransientParent(window_state->window()) == nullptr)
             ? WindowStateType::kMaximized
             : WindowStateType::kNormal;
}
