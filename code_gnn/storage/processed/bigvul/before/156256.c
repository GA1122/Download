void RendererSchedulerImpl::ResumeTimersForAndroidWebView() {
  main_thread_only().pause_timers_for_webview = false;
  UpdatePolicy();
}
