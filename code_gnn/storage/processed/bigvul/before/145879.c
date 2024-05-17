  aura::Window* CreateWindowWithId(const gfx::Rect& bounds, int id) {
    aura::Window* window =
        CreateTestWindowInShellWithDelegate(&delegate_, id, bounds);
    window->SetProperty(aura::client::kTopViewInset, kHeaderHeight);
    return window;
  }
