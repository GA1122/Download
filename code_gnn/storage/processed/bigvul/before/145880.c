  void DragWindowTo(WindowSelectorItem* item, const gfx::Point& end_location) {
    const gfx::Point start_location(item->target_bounds().CenterPoint());
    window_selector()->InitiateDrag(item, start_location);
    window_selector()->Drag(item, end_location);
    window_selector()->CompleteDrag(item, end_location);
  }
