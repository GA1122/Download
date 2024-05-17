  gfx::Rect GetSplitViewDividerBounds(bool is_dragging) {
    if (!split_view_controller()->IsSplitViewModeActive())
      return gfx::Rect();
    return split_view_controller()
        ->split_view_divider_->GetDividerBoundsInScreen(is_dragging);
  }
