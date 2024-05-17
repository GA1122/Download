void ExpandableContainerView::DetailsView::AnimateToState(double state) {
  state_ = state;
  PreferredSizeChanged();
  SchedulePaint();
}
