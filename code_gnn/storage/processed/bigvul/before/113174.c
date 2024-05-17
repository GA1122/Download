int LauncherView::DetermineLastVisibleIndex(int max_value) {
  int index = view_model_->view_size() - 1;
  while (index >= 0 &&
         primary_axis_coordinate(
             view_model_->ideal_bounds(index).right(),
             view_model_->ideal_bounds(index).bottom()) > max_value) {
    index--;
  }
  return index;
}
