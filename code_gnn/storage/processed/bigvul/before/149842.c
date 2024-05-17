void LayerTreeHost::SetHasGpuRasterizationTrigger(bool has_trigger) {
  if (has_trigger == has_gpu_rasterization_trigger_)
    return;

  has_gpu_rasterization_trigger_ = has_trigger;
  TRACE_EVENT_INSTANT1(
      "cc", "LayerTreeHostInProcess::SetHasGpuRasterizationTrigger",
      TRACE_EVENT_SCOPE_THREAD, "has_trigger", has_gpu_rasterization_trigger_);
}
