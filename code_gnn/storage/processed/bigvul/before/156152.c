void RendererSchedulerImpl::AddPendingNavigation(NavigatingFrameType type) {
  helper_.CheckOnValidThread();
  if (type == NavigatingFrameType::kMainFrame) {
    main_thread_only().navigation_task_expected_count++;
    UpdatePolicy();
  }
}
