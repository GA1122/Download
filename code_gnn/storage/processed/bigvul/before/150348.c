void ClientControlledShellSurface::AttemptToStartDrag(
    int component,
    const gfx::Point& location) {
  aura::Window* target = widget_->GetNativeWindow();
  ash::ToplevelWindowEventHandler* toplevel_handler =
      ash::Shell::Get()->toplevel_window_event_handler();
  aura::Window* mouse_pressed_handler =
      target->GetHost()->dispatcher()->mouse_pressed_handler();
  if (toplevel_handler->gesture_target() ||
      (mouse_pressed_handler && target->Contains(mouse_pressed_handler))) {
    gfx::Point point_in_root(location);
    wm::ConvertPointFromScreen(target->GetRootWindow(), &point_in_root);
    toplevel_handler->AttemptToStartDrag(
        target, point_in_root, component,
        ash::ToplevelWindowEventHandler::EndClosure());
  }
}
