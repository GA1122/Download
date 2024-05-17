void RendererSchedulerImpl::RemoveWebViewScheduler(
    WebViewSchedulerImpl* web_view_scheduler) {
  DCHECK(main_thread_only().web_view_schedulers.find(web_view_scheduler) !=
         main_thread_only().web_view_schedulers.end());
  main_thread_only().web_view_schedulers.erase(web_view_scheduler);
}
