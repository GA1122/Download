  base::Optional<int> GetInsertionIndexForDraggedBoundsStacked(
      const gfx::Rect& dragged_bounds,
      bool mouse_has_ever_moved_left,
      bool mouse_has_ever_moved_right) const {
    int active_index = *GetActiveTouchIndex();
    base::Optional<int> index =
        GetInsertionIndexFromReversed(dragged_bounds, active_index);
    if (index != active_index)
      return index;
    if (!index)
      return GetInsertionIndexFrom(dragged_bounds, active_index + 1);

    if (active_index + 1 < GetTabCount() &&
        tab_strip_->touch_layout_->IsStacked(active_index + 1)) {
      index = GetInsertionIndexFrom(dragged_bounds, active_index + 1);
      if (!index && ShouldDragToNextStackedTab(dragged_bounds, active_index,
                                               mouse_has_ever_moved_right))
        index = active_index + 1;
      else if (index == -1)
        index = active_index;
    } else if (ShouldDragToPreviousStackedTab(dragged_bounds, active_index,
                                              mouse_has_ever_moved_left)) {
      index = active_index - 1;
    }
    return index;
  }
