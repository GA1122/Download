RendererSchedulerImpl::PauseRenderer() {
  return std::make_unique<RendererPauseHandleImpl>(this);
}
