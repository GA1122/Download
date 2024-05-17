void Shell::SetShelfAlignment(ShelfAlignment alignment,
                              aura::Window* root_window) {
  ShelfWidget* shelf_widget = GetRootWindowController(root_window)->shelf();
  if (shelf_widget->shelf_layout_manager()->SetAlignment(alignment)) {
    FOR_EACH_OBSERVER(
        ShellObserver, observers_, OnShelfAlignmentChanged(root_window));
  }
}
