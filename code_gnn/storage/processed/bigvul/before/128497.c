void ShellSurface::OnPostWindowStateTypeChange(
    ash::wm::WindowState* window_state,
    ash::wm::WindowStateType old_type) {
  ash::wm::WindowStateType new_type = window_state->GetStateType();
  if (ash::wm::IsMaximizedOrFullscreenOrPinnedWindowStateType(old_type) ||
      ash::wm::IsMaximizedOrFullscreenOrPinnedWindowStateType(new_type)) {
    Configure();
  }

  if (widget_)
    UpdateWidgetBounds();

  if (!state_changed_callback_.is_null())
    state_changed_callback_.Run(old_type, new_type);

  scoped_animations_disabled_.reset();
}
