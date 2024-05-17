  aura::Window* CreateNonActivatableWindow(const gfx::Rect& bounds) {
    aura::Window* window = CreateWindow(bounds);
    ::wm::SetActivationDelegate(window, &non_activatable_activation_delegate_);
    EXPECT_FALSE(wm::CanActivateWindow(window));
    return window;
  }
