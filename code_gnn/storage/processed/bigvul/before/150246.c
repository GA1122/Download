void SetWindowRestoreOverrides(aura::Window* window,
                               const gfx::Rect& bounds_override,
                               ui::WindowShowState window_state_override) {
  if (bounds_override.IsEmpty()) {
    window->ClearProperty(kRestoreWindowStateTypeOverrideKey);
    window->ClearProperty(kRestoreBoundsOverrideKey);
    return;
  }
  window->SetProperty(kRestoreWindowStateTypeOverrideKey,
                      ToWindowStateType(window_state_override));
  window->SetProperty(kRestoreBoundsOverrideKey,
                      new gfx::Rect(bounds_override));
}
