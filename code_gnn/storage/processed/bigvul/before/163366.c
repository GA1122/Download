void RenderThreadImpl::OnMemoryPressure(
    base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level) {
  TRACE_EVENT0("memory", "RenderThreadImpl::OnMemoryPressure");
  if (blink_platform_impl_) {
    blink::WebMemoryCoordinator::OnMemoryPressure(
        static_cast<blink::WebMemoryPressureLevel>(memory_pressure_level));
  }
  if (memory_pressure_level ==
      base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_CRITICAL) {
    ReleaseFreeMemory();
    ClearMemory();
  }
}
