void RendererSchedulerImpl::ResetForNavigationLocked() {
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
               "RendererSchedulerImpl::ResetForNavigationLocked");
  helper_.CheckOnValidThread();
  any_thread_lock_.AssertAcquired();
  any_thread().user_model.Reset(helper_.NowTicks());
  any_thread().have_seen_a_potentially_blocking_gesture = false;
  any_thread().waiting_for_meaningful_paint = true;
  any_thread().have_seen_input_since_navigation = false;
  main_thread_only().loading_task_cost_estimator.Clear();
  main_thread_only().timer_task_cost_estimator.Clear();
  main_thread_only().idle_time_estimator.Clear();
  main_thread_only().have_seen_a_begin_main_frame = false;
  main_thread_only().have_reported_blocking_intervention_since_navigation =
      false;
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    web_view_scheduler->OnNavigation();
  }
  UpdatePolicyLocked(UpdateType::kMayEarlyOutIfPolicyUnchanged);

  UMA_HISTOGRAM_COUNTS_100("RendererScheduler.WebViewsPerScheduler",
                           main_thread_only().web_view_schedulers.size());

  size_t frame_count = 0;
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    frame_count += web_view_scheduler->FrameCount();
  }
  UMA_HISTOGRAM_COUNTS_100("RendererScheduler.WebFramesPerScheduler",
                           frame_count);
}
