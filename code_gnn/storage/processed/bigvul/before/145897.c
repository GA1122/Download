  void IsWindowAndCloseButtonInScreen(aura::Window* window,
                                      WindowSelectorItem* window_item) {
    aura::Window* root_window = window_item->root_window();
    EXPECT_TRUE(window_item->Contains(window));
    EXPECT_TRUE(root_window->GetBoundsInScreen().Contains(
        GetTransformedTargetBounds(window)));
    EXPECT_TRUE(
        root_window->GetBoundsInScreen().Contains(GetTransformedTargetBounds(
            GetCloseButton(window_item)->GetNativeView())));
  }
