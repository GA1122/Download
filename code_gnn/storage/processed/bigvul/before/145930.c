  void SetShelfAutoHideBehavior(aura::Window* window,
                                ShelfAutoHideBehavior behavior) {
    Shelf* shelf = GetShelfForWindow(window);
    shelf->SetAutoHideBehavior(behavior);
    ShelfViewTestAPI test_api(shelf->GetShelfViewForTesting());
    test_api.RunMessageLoopUntilAnimationsDone();
  }
