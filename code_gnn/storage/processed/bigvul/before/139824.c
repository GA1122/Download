ShelfAutoHideBehavior Shell::GetShelfAutoHideBehavior(
    aura::Window* root_window) const {
  ShelfWidget* shelf_widget = GetRootWindowController(root_window)->shelf();
  return shelf_widget->shelf_layout_manager()->auto_hide_behavior();
}
