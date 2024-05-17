void RendererSchedulerImpl::OnTraceLogEnabled() {
  CreateTraceEventObjectSnapshot();
  tracing_controller_.OnTraceLogEnabled();
  for (WebViewSchedulerImpl* web_view_scheduler :
      main_thread_only().web_view_schedulers) {
    web_view_scheduler->OnTraceLogEnabled();
  }
}
