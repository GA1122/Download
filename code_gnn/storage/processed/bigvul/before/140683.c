bool IsDockedModeEnabled() {
  display::DisplayManager* display_manager = GetDisplayManager();
  if (!display::Display::HasInternalDisplay())
    return false;

  for (size_t i = 0; i < display_manager->GetNumDisplays(); ++i) {
    if (display::Display::IsInternalDisplayId(
            display_manager->GetDisplayAt(i).id())) {
      return false;
    }
  }

  return true;
}
