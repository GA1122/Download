  base::Optional<int> GetInsertionIndexFrom(const gfx::Rect& dragged_bounds,
                                            int start) const {
    const int last_tab = GetTabCount() - 1;
    const int dragged_x = GetDraggedX(dragged_bounds);
    if (start < 0 || start > last_tab || dragged_x < ideal_bounds(start).x() ||
        dragged_x > ideal_bounds(last_tab).right())
      return base::nullopt;

    for (int i = start; i <= last_tab; ++i) {
      if (dragged_x < ideal_bounds(i).CenterPoint().x())
        return i;
    }

    return last_tab + 1;
  }
