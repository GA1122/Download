void RendererSchedulerImpl::OnTriedToExecuteBlockedTask() {
  if (main_thread_only().current_use_case == UseCase::kTouchstart ||
      main_thread_only().longest_jank_free_task_duration <
          base::TimeDelta::FromMilliseconds(kRailsResponseTimeMillis) ||
      main_thread_only().renderer_pause_count != 0 ||
      main_thread_only().stopped_when_backgrounded) {
    return;
  }
  if (!main_thread_only().timer_tasks_seem_expensive &&
      !main_thread_only().loading_tasks_seem_expensive) {
    return;
  }
  if (!main_thread_only()
           .have_reported_blocking_intervention_in_current_policy) {
    main_thread_only().have_reported_blocking_intervention_in_current_policy =
        true;
    TRACE_EVENT_INSTANT0("renderer.scheduler",
                         "RendererSchedulerImpl::TaskBlocked",
                         TRACE_EVENT_SCOPE_THREAD);
  }

  if (!main_thread_only()
           .have_reported_blocking_intervention_since_navigation) {
    {
      base::AutoLock lock(any_thread_lock_);
      if (!any_thread().have_seen_a_potentially_blocking_gesture)
        return;
    }
    main_thread_only().have_reported_blocking_intervention_since_navigation =
        true;
    BroadcastIntervention(
        "Blink deferred a task in order to make scrolling smoother. "
        "Your timer and network tasks should take less than 50ms to run "
        "to avoid this. Please see "
        "https://developers.google.com/web/tools/chrome-devtools/profile/"
        "evaluate-performance/rail"
        " and https://crbug.com/574343#c40 for more information.");
  }
}
