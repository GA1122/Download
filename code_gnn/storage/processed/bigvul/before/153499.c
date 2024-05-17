int TabStrip::UpdateIdealBoundsForPinnedTabs(int* first_non_pinned_index) {
  layout_helper_->UpdateIdealBoundsForPinnedTabs(&tabs_);
  if (first_non_pinned_index)
    *first_non_pinned_index = layout_helper_->first_non_pinned_tab_index();
  return layout_helper_->first_non_pinned_tab_x();
}
