void RendererSchedulerImpl::SetStoppedInBackground(bool stopped) const {
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    web_view_scheduler->SetPageStopped(stopped);
  }
}
