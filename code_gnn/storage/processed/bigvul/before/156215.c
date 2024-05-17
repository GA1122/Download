bool RendererSchedulerImpl::TaskQueuePolicy::IsQueueEnabled(
    MainThreadTaskQueue* task_queue) const {
  if (!is_enabled)
    return false;
  if (is_paused && task_queue->CanBePaused())
    return false;
  if (is_blocked && task_queue->CanBeDeferred())
    return false;
  if (is_stopped && task_queue->CanBeStopped())
    return false;
  return true;
}
