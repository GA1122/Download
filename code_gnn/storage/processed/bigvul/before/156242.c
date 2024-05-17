void RendererSchedulerImpl::PauseTimersForAndroidWebView() {
  main_thread_only().pause_timers_for_webview = true;
  UpdatePolicy();
}
