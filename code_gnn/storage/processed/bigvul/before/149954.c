gfx::ColorSpace LayerTreeHostImpl::GetTileColorSpace() const {
  if (!sync_tree())
    return gfx::ColorSpace::CreateSRGB();
  return sync_tree()->device_color_space();
}
