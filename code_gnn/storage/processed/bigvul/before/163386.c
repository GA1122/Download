void RenderThreadImpl::RecordPurgeAndSuspendMemoryGrowthMetrics(
    const char* suffix,
    int foregrounded_count_when_purged) {
  if (!RendererIsHidden())
    return;
  if (foregrounded_count_when_purged != process_foregrounded_count_)
    return;

  RendererMemoryMetrics memory_metrics;
  if (!GetRendererMemoryMetrics(&memory_metrics))
    return;

  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.PartitionAllocKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        partition_alloc_kb));
  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.BlinkGCKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        blink_gc_kb));
  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.MallocKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        malloc_mb) *
          1024);
  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.DiscardableKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        discardable_kb));
  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.V8MainThreadIsolateKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        v8_main_thread_isolate_mb) *
          1024);
  RecordPurgeAndSuspendMemoryGrowthKB(
      "PurgeAndSuspend.Experimental.MemoryGrowth.TotalAllocatedKB", suffix,
      GET_MEMORY_GROWTH(memory_metrics, purge_and_suspend_memory_metrics_,
                        total_allocated_mb) *
          1024);
}
