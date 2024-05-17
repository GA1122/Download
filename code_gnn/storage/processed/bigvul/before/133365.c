void WindowTreeHostManager::InitHosts() {
  RootWindowController::CreateForPrimaryDisplay(
      window_tree_hosts_[primary_display_id]);
  DisplayManager* display_manager = GetDisplayManager();
  for (size_t i = 0; i < display_manager->GetNumDisplays(); ++i) {
    const display::Display& display = display_manager->GetDisplayAt(i);
    if (primary_display_id != display.id()) {
      AshWindowTreeHost* ash_host =
          AddWindowTreeHostForDisplay(display, AshWindowTreeHostInitParams());
      RootWindowController::CreateForSecondaryDisplay(ash_host);
    }
  }

  FOR_EACH_OBSERVER(Observer, observers_, OnDisplaysInitialized());
}
