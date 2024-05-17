void ClientControlledShellSurface::SetOrientationLock(
    ash::OrientationLockType orientation_lock) {
  TRACE_EVENT1("exo", "ClientControlledShellSurface::SetOrientationLock",
               "orientation_lock", static_cast<int>(orientation_lock));

  if (!widget_) {
    initial_orientation_lock_ = orientation_lock;
    return;
  }

  ash::Shell* shell = ash::Shell::Get();
  shell->screen_orientation_controller()->LockOrientationForWindow(
      widget_->GetNativeWindow(), orientation_lock);
}
