void RendererSchedulerImpl::BroadcastIntervention(const std::string& message) {
  helper_.CheckOnValidThread();
  for (auto* web_view_scheduler : main_thread_only().web_view_schedulers)
    web_view_scheduler->ReportIntervention(message);
}
