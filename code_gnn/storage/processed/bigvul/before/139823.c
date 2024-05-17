ShelfAlignment Shell::GetShelfAlignment(const aura::Window* root_window) {
  ShelfWidget* shelf_widget = GetRootWindowController(root_window)->shelf();
  return shelf_widget->shelf_layout_manager()->GetAlignment();
}
