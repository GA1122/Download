RendererSchedulerImpl::GetTimerTaskCostEstimatorForTesting() {
  return &main_thread_only().timer_task_cost_estimator;
}
