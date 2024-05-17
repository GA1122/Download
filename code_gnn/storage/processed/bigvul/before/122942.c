void RenderWidgetHostImpl::CompositingSurfaceUpdated() {
  GpuSurfaceTracker::Get()->SetSurfaceHandle(
      surface_id_, GetCompositingSurface());
  process_->SurfaceUpdated(surface_id_);
}
