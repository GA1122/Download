  explicit ClientControlledWindowStateDelegate(
      ClientControlledShellSurface* shell_surface,
      ash::wm::ClientControlledState::Delegate* delegate)
      : shell_surface_(shell_surface), delegate_(delegate) {}
