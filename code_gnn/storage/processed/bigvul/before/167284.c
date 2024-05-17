views::View* ScrollableShelfView::FindFirstFocusableChild() {
  return shelf_view_->view_model()->view_at(shelf_view_->first_visible_index());
}
