void GpuCommandBufferStub::OnDiscardBackbuffer() {
  if (!surface_)
    return;
  if (allocation_.suggest_have_frontbuffer)
    surface_->SetBufferAllocation(
        gfx::GLSurface::BUFFER_ALLOCATION_FRONT_ONLY);
  else
    surface_->SetBufferAllocation(
        gfx::GLSurface::BUFFER_ALLOCATION_NONE);
}
