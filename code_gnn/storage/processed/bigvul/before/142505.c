  const ui::Layer* GetNonLockScreenContainersContainerLayer() const {
    const auto* shelf_window = GetShelfWidget()->GetNativeWindow();
    return shelf_window->GetRootWindow()
        ->GetChildById(kShellWindowId_NonLockScreenContainersContainer)
        ->layer();
  }
