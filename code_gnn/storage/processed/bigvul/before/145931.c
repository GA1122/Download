  void SetShelfVisibilityState(aura::Window* window,
                               ShelfVisibilityState visibility_state) {
    Shelf* shelf = GetShelfForWindow(window);
    shelf->shelf_layout_manager()->SetState(visibility_state);
  }
