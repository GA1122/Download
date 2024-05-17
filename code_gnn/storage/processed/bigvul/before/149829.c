void LayerTreeHost::SetBottomControlsHeight(float height) {
  if (bottom_controls_height_ == height)
    return;

  bottom_controls_height_ = height;
  SetNeedsCommit();
}
