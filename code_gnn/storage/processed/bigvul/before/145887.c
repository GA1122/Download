  aura::Window* GetOverviewWindowForMinimizedState(int index,
                                                   aura::Window* window) {
    WindowSelectorItem* selector = GetWindowItemForWindow(index, window);
    return selector->GetOverviewWindowForMinimizedStateForTest();
  }
