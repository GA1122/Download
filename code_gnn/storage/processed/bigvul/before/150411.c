void ClientControlledShellSurface::UpdateBackdrop() {
  aura::Window* window = widget_->GetNativeWindow();

  bool enable_backdrop = (widget_->IsFullscreen() || widget_->IsMaximized());

  ash::BackdropWindowMode target_backdrop_mode =
      enable_backdrop ? ash::BackdropWindowMode::kEnabled
                      : ash::BackdropWindowMode::kAuto;

  if (window->GetProperty(ash::kBackdropWindowMode) != target_backdrop_mode)
    window->SetProperty(ash::kBackdropWindowMode, target_backdrop_mode);
}
