void RendererSchedulerImpl::OnPendingTasksChanged(bool has_tasks) {
  if (has_tasks ==
      main_thread_only().compositor_will_send_main_frame_not_expected.get())
    return;

  main_thread_only().compositor_will_send_main_frame_not_expected = has_tasks;

  TRACE_EVENT1(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
               "RendererSchedulerImpl::OnPendingTasksChanged", "has_tasks",
               has_tasks);
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    web_view_scheduler->RequestBeginMainFrameNotExpected(has_tasks);
  }
}
