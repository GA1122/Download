void WindowTreeHostManager::OnHostResized(const aura::WindowTreeHost* host) {
  display::Display display =
      display::Screen::GetScreen()->GetDisplayNearestWindow(
          const_cast<aura::Window*>(host->window()));

  DisplayManager* display_manager = GetDisplayManager();
  if (display_manager->UpdateDisplayBounds(display.id(), host->GetBounds())) {
    mirror_window_controller_->UpdateWindow();
    cursor_window_controller_->UpdateContainer();
  }
}
