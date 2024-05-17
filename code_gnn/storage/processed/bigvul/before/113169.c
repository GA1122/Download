int LauncherView::CancelDrag(int modified_index) {
  if (!drag_view_)
    return modified_index;
  bool was_dragging = dragging_;
  int drag_view_index = view_model_->GetIndexOfView(drag_view_);
  dragging_ = false;
  drag_view_ = NULL;
  if (drag_view_index == modified_index) {
    return modified_index;
  }
  if (!was_dragging)
    return modified_index;

  views::View* removed_view =
      (modified_index >= 0) ? view_model_->view_at(modified_index) : NULL;
  model_->Move(drag_view_index, start_drag_index_);
  return removed_view ? view_model_->GetIndexOfView(removed_view) : -1;
}
