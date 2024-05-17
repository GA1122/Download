void RendererSchedulerImpl::SetRendererHidden(bool hidden) {
  if (hidden) {
    TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
                 "RendererSchedulerImpl::OnRendererHidden");
  } else {
    TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
                 "RendererSchedulerImpl::OnRendererVisible");
  }
  helper_.CheckOnValidThread();
  main_thread_only().renderer_hidden = hidden;
}
