void ClientControlledShellSurface::OnSetFrameColors(SkColor active_color,
                                                    SkColor inactive_color) {
  ShellSurfaceBase::OnSetFrameColors(active_color, inactive_color);
  if (wide_frame_) {
    aura::Window* window = wide_frame_->GetWidget()->GetNativeWindow();
    window->SetProperty(ash::kFrameActiveColorKey, active_color);
    window->SetProperty(ash::kFrameInactiveColorKey, inactive_color);
  }
}
