  void DetachReattachTest(aura::Window* window, int dx, int dy) {
    EXPECT_TRUE(window->GetProperty(kPanelAttachedKey));
    aura::Window* root_window = window->GetRootWindow();
    EXPECT_EQ(kShellWindowId_PanelContainer, window->parent()->id());
    DragStart(window);
    gfx::Rect initial_bounds = window->GetBoundsInScreen();

    DragMove(dx * 5, dy * 5);
    EXPECT_EQ(initial_bounds.x(), window->GetBoundsInScreen().x());
    EXPECT_EQ(initial_bounds.y(), window->GetBoundsInScreen().y());

    DragMove(dx * 100, dy * 100);
    EXPECT_EQ(initial_bounds.x() + dx * 100, window->GetBoundsInScreen().x());
    EXPECT_EQ(initial_bounds.y() + dy * 100, window->GetBoundsInScreen().y());
    DragEnd();

    EXPECT_FALSE(window->GetProperty(kPanelAttachedKey));
    EXPECT_EQ(kShellWindowId_DefaultContainer, window->parent()->id());
    EXPECT_EQ(root_window, window->GetRootWindow());

    DragStart(window);
    DragMove(dx * -95, dy * -95);
    DragEnd();

    EXPECT_TRUE(window->GetProperty(kPanelAttachedKey));
    EXPECT_EQ(initial_bounds.x(), window->GetBoundsInScreen().x());
    EXPECT_EQ(initial_bounds.y(), window->GetBoundsInScreen().y());
    EXPECT_EQ(kShellWindowId_PanelContainer, window->parent()->id());
  }
