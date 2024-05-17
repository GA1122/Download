void RendererSchedulerImpl::AddQueueToWakeUpBudgetPool(
    MainThreadTaskQueue* queue) {
  if (!main_thread_only().wake_up_budget_pool) {
    main_thread_only().wake_up_budget_pool =
        task_queue_throttler()->CreateWakeUpBudgetPool("renderer_wake_up_pool");
    main_thread_only().wake_up_budget_pool->SetWakeUpRate(1);
    main_thread_only().wake_up_budget_pool->SetWakeUpDuration(
        GetWakeUpDuration());
  }
  main_thread_only().wake_up_budget_pool->AddQueue(tick_clock()->NowTicks(),
                                                   queue);
}
