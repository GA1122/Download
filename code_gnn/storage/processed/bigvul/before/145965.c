  aura::Window* CreateWindow(aura::client::WindowType type,
                             const gfx::Rect bounds) {
    InitParams params(type);
    params.bounds = bounds;
    return CreateWindowInWatchedContainer(params);
  }
