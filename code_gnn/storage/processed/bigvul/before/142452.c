bool ShelfLayoutManager::IsVisible() const {
  return shelf_widget_->status_area_widget() &&
         shelf_widget_->status_area_widget()->IsVisible() &&
         (state_.visibility_state == SHELF_VISIBLE ||
          (state_.visibility_state == SHELF_AUTO_HIDE &&
           state_.auto_hide_state == SHELF_AUTO_HIDE_SHOWN));
}
