ui::EventDispatchDetails WindowTreeHostManager::DispatchKeyEventPostIME(
    ui::KeyEvent* event) {
  aura::Window* active_window = wm::GetActiveWindow();
  aura::Window* root_window = active_window ? active_window->GetRootWindow()
                                            : Shell::GetPrimaryRootWindow();
  return root_window->GetHost()->DispatchKeyEventPostIME(event);
}
