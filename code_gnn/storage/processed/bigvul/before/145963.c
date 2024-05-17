  aura::Window* CreateNonMaximizableWindow(aura::client::WindowType type,
                                           const gfx::Rect& bounds,
                                           const gfx::Size& max_size) {
    InitParams params(type);
    params.bounds = bounds;
    params.max_size = max_size;
    params.can_maximize = false;
    return CreateWindowInWatchedContainer(params);
  }
