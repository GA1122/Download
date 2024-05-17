bool LayerTreeHost::InitializeOutputSurfaceIfNeeded() {
  if (!output_surface_can_be_initialized_)
    return false;

  if (output_surface_lost_)
    proxy_->CreateAndInitializeOutputSurface();
  return !output_surface_lost_;
}
