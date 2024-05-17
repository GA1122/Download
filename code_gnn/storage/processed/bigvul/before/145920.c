  Shelf* GetShelfForWindow(aura::Window* window) {
    return RootWindowController::ForWindow(window)->shelf();
  }
