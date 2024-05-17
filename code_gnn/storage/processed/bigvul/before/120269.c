void LayerTreeHost::SetOverdrawBottomHeight(float overdraw_bottom_height) {
  if (overdraw_bottom_height_ == overdraw_bottom_height)
    return;

  overdraw_bottom_height_ = overdraw_bottom_height;
  SetNeedsCommit();
}
