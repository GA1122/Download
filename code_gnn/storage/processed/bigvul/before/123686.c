void GpuCommandBufferStub::SetMemoryAllocation(
    const GpuMemoryAllocation& allocation) {
  Send(new GpuCommandBufferMsg_SetMemoryAllocation(
      route_id_, allocation.renderer_allocation));
  if (!surface_ || !MakeCurrent())
    return;
  surface_->SetFrontbufferAllocation(
      allocation.browser_allocation.suggest_have_frontbuffer);
}
