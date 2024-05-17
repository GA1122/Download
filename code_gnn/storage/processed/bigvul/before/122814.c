 void GpuProcessHostUIShim::OnVideoMemoryUsageStatsReceived(
    const GPUVideoMemoryUsageStats& video_memory_usage_stats) {
  GpuDataManagerImpl::GetInstance()->UpdateVideoMemoryUsageStats(
      video_memory_usage_stats);
}
