void Browser::ToolbarSizeChanged(TabContents* source, bool is_animating) {
  if (source == GetSelectedTabContents() || source == NULL) {
    window_->SelectedTabToolbarSizeChanged(is_animating);
  }
}
