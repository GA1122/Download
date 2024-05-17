void ScrollableShelfView::UpdateTappableIconIndices() {
  if (layout_strategy_ == kNotShowArrowButtons) {
    first_tappable_app_index_ = shelf_view_->first_visible_index();
    last_tappable_app_index_ = shelf_view_->last_visible_index();
    return;
  }

  int actual_scroll_distance = GetActualScrollOffset();
  int shelf_container_available_space =
      (GetShelf()->IsHorizontalAlignment() ? shelf_container_view_->width()
                                           : shelf_container_view_->height()) -
      GetFadeZoneLength();
  if (layout_strategy_ == kShowRightArrowButton ||
      layout_strategy_ == kShowButtons) {
    first_tappable_app_index_ = actual_scroll_distance / GetUnit();
    last_tappable_app_index_ =
        first_tappable_app_index_ + shelf_container_available_space / GetUnit();
  } else {
    DCHECK_EQ(layout_strategy_, kShowLeftArrowButton);
    last_tappable_app_index_ = shelf_view_->last_visible_index();
    first_tappable_app_index_ =
        last_tappable_app_index_ - shelf_container_available_space / GetUnit();
  }
}
