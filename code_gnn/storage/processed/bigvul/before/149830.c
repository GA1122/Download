void LayerTreeHost::SetBrowserControlsHeight(float height, bool shrink) {
  if (top_controls_height_ == height &&
      browser_controls_shrink_blink_size_ == shrink)
    return;

  top_controls_height_ = height;
  browser_controls_shrink_blink_size_ = shrink;
  SetNeedsCommit();
}
