bool RenderThreadImpl::GetRendererMemoryMetrics(
    RendererMemoryMetrics* memory_metrics) const {
  DCHECK(memory_metrics);

  size_t render_view_count = RenderView::GetRenderViewCount();

  if (render_view_count == 0)
    return false;

  blink::WebMemoryStatistics blink_stats = blink::WebMemoryStatistics::Get();
  memory_metrics->partition_alloc_kb =
      blink_stats.partition_alloc_total_allocated_bytes / 1024;
  memory_metrics->blink_gc_kb =
      blink_stats.blink_gc_total_allocated_bytes / 1024;
  std::unique_ptr<base::ProcessMetrics> metric(
      base::ProcessMetrics::CreateCurrentProcessMetrics());
  size_t malloc_usage = metric->GetMallocUsage();
  memory_metrics->malloc_mb = malloc_usage / 1024 / 1024;

  discardable_memory::ClientDiscardableSharedMemoryManager::Statistics
      discardable_stats = discardable_shared_memory_manager_->GetStatistics();
  size_t discardable_usage =
      discardable_stats.total_size - discardable_stats.freelist_size;
  memory_metrics->discardable_kb = discardable_usage / 1024;

  size_t v8_usage = 0;
  if (v8::Isolate* isolate = blink::MainThreadIsolate()) {
    v8::HeapStatistics v8_heap_statistics;
    isolate->GetHeapStatistics(&v8_heap_statistics);
    v8_usage = v8_heap_statistics.total_heap_size();
  }
  memory_metrics->v8_main_thread_isolate_mb = v8_usage / 1024 / 1024;
  size_t total_allocated = blink_stats.partition_alloc_total_allocated_bytes +
                           blink_stats.blink_gc_total_allocated_bytes +
                           malloc_usage + v8_usage + discardable_usage;
  memory_metrics->total_allocated_mb = total_allocated / 1024 / 1024;
  memory_metrics->non_discardable_total_allocated_mb =
      (total_allocated - discardable_usage) / 1024 / 1024;
  memory_metrics->total_allocated_per_render_view_mb =
      total_allocated / render_view_count / 1024 / 1024;

  return true;
}
