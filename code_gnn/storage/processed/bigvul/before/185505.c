  bool CanHandleMagnifyScreen() {
  Shell* shell = Shell::GetInstance();
  return shell->magnification_controller()->IsEnabled() ||
         shell->partial_magnification_controller()->is_enabled();
//   return Shell::GetInstance()->magnification_controller()->IsEnabled();
  }