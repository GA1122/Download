void RecordSuffixedRendererMemoryMetrics(
    const RenderThreadImpl::RendererMemoryMetrics& memory_metrics,
    base::StringPiece suffix) {
  RecordSuffixedMemoryMBHistogram("Memory.Experimental.Renderer.PartitionAlloc",
                                  suffix,
                                  memory_metrics.partition_alloc_kb / 1024);
  RecordSuffixedMemoryMBHistogram("Memory.Experimental.Renderer.BlinkGC",
                                  suffix, memory_metrics.blink_gc_kb / 1024);
  RecordSuffixedMemoryMBHistogram("Memory.Experimental.Renderer.Malloc", suffix,
                                  memory_metrics.malloc_mb);
  RecordSuffixedMemoryMBHistogram("Memory.Experimental.Renderer.Discardable",
                                  suffix, memory_metrics.discardable_kb / 1024);
  RecordSuffixedMemoryMBHistogram(
      "Memory.Experimental.Renderer.V8MainThreadIsolate", suffix,
      memory_metrics.v8_main_thread_isolate_mb);
  RecordSuffixedMemoryMBHistogram("Memory.Experimental.Renderer.TotalAllocated",
                                  suffix, memory_metrics.total_allocated_mb);
  RecordSuffixedMemoryMBHistogram(
      "Memory.Experimental.Renderer.NonDiscardableTotalAllocated", suffix,
      memory_metrics.non_discardable_total_allocated_mb);
  RecordSuffixedMemoryMBHistogram(
      "Memory.Experimental.Renderer.TotalAllocatedPerRenderView", suffix,
      memory_metrics.total_allocated_per_render_view_mb);
}
