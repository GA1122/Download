RendererSchedulerImpl::RendererPauseHandleImpl::~RendererPauseHandleImpl() {
  scheduler_->ResumeRendererImpl();
}
