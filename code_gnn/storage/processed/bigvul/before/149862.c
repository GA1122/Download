void LayerTreeHost::SetViewportSize(const gfx::Size& device_viewport_size) {
  if (device_viewport_size_ == device_viewport_size)
    return;

  device_viewport_size_ = device_viewport_size;

  SetPropertyTreesNeedRebuild();
  SetNeedsCommit();
}
