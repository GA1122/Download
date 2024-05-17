bool PixelBufferRasterWorkerPool::HasPendingTasks() const {
  return PendingRasterTaskCount() || !tasks_with_pending_upload_.empty();
}
