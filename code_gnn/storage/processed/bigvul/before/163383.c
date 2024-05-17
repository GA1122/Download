void RenderThreadImpl::RecordMemoryUsageAfterBackgrounded(
    const char* suffix,
    int foregrounded_count) {
  if (!RendererIsHidden())
    return;
  if (foregrounded_count != process_foregrounded_count_)
    return;

  RendererMemoryMetrics memory_metrics;
  if (!GetRendererMemoryMetrics(&memory_metrics))
    return;
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.PartitionAlloc.AfterBackgrounded", suffix,
      memory_metrics.partition_alloc_kb / 1024);
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.BlinkGC.AfterBackgrounded", suffix,
      memory_metrics.blink_gc_kb / 1024);
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.Malloc.AfterBackgrounded", suffix,
      memory_metrics.malloc_mb);
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.Discardable.AfterBackgrounded", suffix,
      memory_metrics.discardable_kb / 1024);
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.V8MainThreaIsolate.AfterBackgrounded",
      suffix, memory_metrics.v8_main_thread_isolate_mb);
  RecordMemoryUsageAfterBackgroundedMB(
      "Memory.Experimental.Renderer.TotalAllocated.AfterBackgrounded", suffix,
      memory_metrics.total_allocated_mb);
}
