const gfx::Rect LayerTreeHostImpl::ViewportRectForTilePriority() const {
  if (viewport_rect_for_tile_priority_.IsEmpty())
    return DeviceViewport();

  return viewport_rect_for_tile_priority_;
}
