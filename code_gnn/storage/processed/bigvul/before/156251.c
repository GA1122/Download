RendererSchedulerImpl::RendererPauseHandleImpl::RendererPauseHandleImpl(
    RendererSchedulerImpl* scheduler)
    : scheduler_(scheduler) {
  scheduler_->PauseRendererImpl();
}
