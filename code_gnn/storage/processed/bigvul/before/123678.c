void GpuCommandBufferStub::OnSetSurfaceVisible(bool visible) {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnSetSurfaceVisible");
  GetMemoryManager()->
      SetClientVisible(this, visible);
}
