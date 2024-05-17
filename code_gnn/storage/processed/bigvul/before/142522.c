  ShelfDragCallback(const gfx::Rect& auto_hidden_shelf_bounds,
                    const gfx::Rect& visible_shelf_bounds)
      : auto_hidden_shelf_bounds_(auto_hidden_shelf_bounds),
        visible_shelf_bounds_(visible_shelf_bounds),
        was_visible_on_drag_start_(false) {
    EXPECT_EQ(auto_hidden_shelf_bounds_.size(), visible_shelf_bounds_.size());
  }
