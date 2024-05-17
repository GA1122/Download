    GpuCommandBufferStub::surface_state() const {
  DCHECK(has_surface_state());
  return *surface_state_.get();
}
