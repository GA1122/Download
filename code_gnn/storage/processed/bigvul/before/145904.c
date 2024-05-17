  bool WindowsOverlapping(aura::Window* window1, aura::Window* window2) {
    gfx::Rect window1_bounds = GetTransformedTargetBounds(window1);
    gfx::Rect window2_bounds = GetTransformedTargetBounds(window2);
    return window1_bounds.Intersects(window2_bounds);
  }
