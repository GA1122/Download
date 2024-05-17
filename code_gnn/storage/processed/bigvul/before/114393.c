void GpuCommandBufferStub::OnEnsureBackbuffer() {
  if (!surface_)
    return;
  surface_->SetBufferAllocation(
      gfx::GLSurface::BUFFER_ALLOCATION_FRONT_AND_BACK);
}
