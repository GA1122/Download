  aura::Window* CreatePanelWindow(const gfx::Rect& bounds) {
    aura::Window* window = CreateTestWindowInShellWithDelegateAndType(
        nullptr, aura::client::WINDOW_TYPE_PANEL, 0, bounds);
    static int id = 0;
    std::string shelf_id(ShelfID(base::IntToString(id++)).Serialize());
    window->SetProperty(kShelfIDKey, new std::string(shelf_id));
    window->SetProperty<int>(kShelfItemTypeKey, TYPE_APP_PANEL);
    window->SetProperty(aura::client::kTopViewInset, kHeaderHeight);
    shelf_view_test()->RunMessageLoopUntilAnimationsDone();
    return window;
  }
