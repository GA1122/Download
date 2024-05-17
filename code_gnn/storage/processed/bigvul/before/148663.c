void SkiaOutputSurfaceImpl::ScheduleGpuTaskForTesting(
    base::OnceClosure callback,
    std::vector<gpu::SyncToken> sync_tokens) {
  ScheduleGpuTask(std::move(callback), std::move(sync_tokens));
}
