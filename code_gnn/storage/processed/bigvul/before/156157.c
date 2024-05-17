void RendererSchedulerImpl::AddWebViewScheduler(
    WebViewSchedulerImpl* web_view_scheduler) {
  main_thread_only().web_view_schedulers.insert(web_view_scheduler);
}
