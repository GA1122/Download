  aura::Window* CreateWindow(const gfx::Rect& bounds) {
    aura::Window* window =
        CreateTestWindowInShellWithDelegate(&delegate_, -1, bounds);
    window->SetProperty(aura::client::kTopViewInset, kHeaderHeight);
    return window;
  }
