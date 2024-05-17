  explicit ShellSurfaceWindowObserver(aura::Window* window)
      : window_(window),
        has_delegate_(ash::wm::GetWindowState(window)->HasDelegate()) {
    window_->AddObserver(this);
  }
