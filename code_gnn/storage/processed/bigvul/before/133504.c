void ScreenPositionController::ConvertPointFromScreen(
    const aura::Window* window,
    gfx::Point* point) {
  const aura::Window* root = window->GetRootWindow();
  const gfx::Point display_origin = Shell::GetScreen()->GetDisplayNearestWindow(
      const_cast<aura::Window*>(root)).bounds().origin();
  point->Offset(-display_origin.x(), -display_origin.y());
  aura::Window::ConvertPointToTarget(root, window, point);
}
