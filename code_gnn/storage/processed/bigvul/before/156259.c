void RendererSchedulerImpl::SetMaxVirtualTimeTaskStarvationCount(
    int max_task_starvation_count) {
  main_thread_only().max_virtual_time_task_starvation_count =
      max_task_starvation_count;
  ApplyVirtualTimePolicy();
}
