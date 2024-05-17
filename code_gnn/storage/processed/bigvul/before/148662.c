void SkiaOutputSurfaceImpl::ScheduleGpuTask(
    base::OnceClosure callback,
    std::vector<gpu::SyncToken> sync_tokens) {
  task_sequence_->ScheduleTask(std::move(callback), std::move(sync_tokens));
}
