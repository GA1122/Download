  gfx::Rect GetGridBounds() {
    if (window_selector())
      return window_selector()->grid_list_[0]->bounds_;

    return gfx::Rect();
  }
