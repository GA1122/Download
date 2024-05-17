views::View* ScrollableShelfView::FindLastFocusableChild() {
   return shelf_view_->view_model()->view_at(shelf_view_->last_visible_index());
 }
