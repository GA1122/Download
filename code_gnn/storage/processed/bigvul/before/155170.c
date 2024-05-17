bool OmniboxViewViews::ShouldDoLearning() {
  return location_bar_view_ && !location_bar_view_->profile()->IsOffTheRecord();
}
