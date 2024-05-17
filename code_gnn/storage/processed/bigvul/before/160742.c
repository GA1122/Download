void RenderFrameImpl::UpdatePeakMemoryStats() {
  if (!base::FeatureList::IsEnabled(features::kReportRendererPeakMemoryStats))
    return;

  RenderThreadImpl::RendererMemoryMetrics memory_metrics;
  if (!RenderThreadImpl::current()->GetRendererMemoryMetrics(&memory_metrics))
    return;
  peak_memory_metrics_.partition_alloc_kb =
      std::max(peak_memory_metrics_.partition_alloc_kb,
               memory_metrics.partition_alloc_kb);
  peak_memory_metrics_.blink_gc_kb =
      std::max(peak_memory_metrics_.blink_gc_kb, memory_metrics.blink_gc_kb);
  peak_memory_metrics_.malloc_mb =
      std::max(peak_memory_metrics_.malloc_mb, memory_metrics.malloc_mb);
  peak_memory_metrics_.discardable_kb = std::max(
      peak_memory_metrics_.discardable_kb, memory_metrics.discardable_kb);
  peak_memory_metrics_.v8_main_thread_isolate_mb =
      std::max(peak_memory_metrics_.v8_main_thread_isolate_mb,
               memory_metrics.v8_main_thread_isolate_mb);
  peak_memory_metrics_.total_allocated_mb =
      std::max(peak_memory_metrics_.total_allocated_mb,
               memory_metrics.total_allocated_mb);
  peak_memory_metrics_.non_discardable_total_allocated_mb =
      std::max(peak_memory_metrics_.non_discardable_total_allocated_mb,
               memory_metrics.non_discardable_total_allocated_mb);
  peak_memory_metrics_.total_allocated_per_render_view_mb =
      std::max(peak_memory_metrics_.total_allocated_per_render_view_mb,
               memory_metrics.total_allocated_per_render_view_mb);
}
