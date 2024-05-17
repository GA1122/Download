void Shell::SetShelfAutoHideBehavior(ShelfAutoHideBehavior behavior,
                                     aura::Window* root_window) {
  ShelfWidget* shelf_widget = GetRootWindowController(root_window)->shelf();
  shelf_widget->shelf_layout_manager()->SetAutoHideBehavior(behavior);
}
