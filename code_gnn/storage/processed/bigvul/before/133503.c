void ScreenPositionController::ConvertHostPointToScreen(
    aura::Window* root_window,
    gfx::Point* point) {
  aura::Window* root = root_window->GetRootWindow();
  aura::Window* target_root = nullptr;
  ConvertHostPointToRelativeToRootWindow(root, Shell::GetAllRootWindows(),
                                         point, &target_root);
  ConvertPointToScreen(target_root, point);
}
