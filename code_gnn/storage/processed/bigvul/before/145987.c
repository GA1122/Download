  aura::Window* CreatePanelWindow() {
    gfx::Rect rect(100, 100);
    aura::Window* window = CreateTestWindowInShellWithDelegateAndType(
        NULL, aura::client::WINDOW_TYPE_PANEL, 0, rect);
    shelf_view_test_->RunMessageLoopUntilAnimationsDone();
    return window;
  }
