void ExpandableContainerView::ToggleDetailLevel() {
  expanded_ = !expanded_;

  if (slide_animation_.IsShowing())
    slide_animation_.Hide();
  else
    slide_animation_.Show();
}
