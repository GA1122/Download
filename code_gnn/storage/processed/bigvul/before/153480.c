void TabStrip::StartPinnedTabAnimation() {
  in_tab_close_ = false;
  available_width_for_tabs_ = -1;

  PrepareForAnimation();

  UpdateIdealBounds();
  AnimateToIdealBounds();
}
