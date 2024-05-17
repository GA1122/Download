  aura::Window* CreatePanelWindow(const gfx::Point& origin) {
    gfx::Rect bounds(origin, gfx::Size(101, 101));
    aura::Window* window = CreateTestWindowInShellWithDelegateAndType(
        NULL, aura::client::WINDOW_TYPE_PANEL, 0, bounds);
    static int id = 0;
    std::string shelf_id(ash::ShelfID(base::IntToString(id++)).Serialize());
    window->SetProperty(kShelfIDKey, new std::string(shelf_id));
    window->SetProperty<int>(kShelfItemTypeKey, TYPE_APP_PANEL);
    shelf_view_test_->RunMessageLoopUntilAnimationsDone();
    return window;
  }
