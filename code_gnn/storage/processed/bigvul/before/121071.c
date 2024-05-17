bool PixelBufferRasterWorkerPool::HasPendingTasksRequiredForActivation() const {
  return !tasks_required_for_activation_.empty();
}
