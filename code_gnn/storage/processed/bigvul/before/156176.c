std::unique_ptr<blink::WebThread> RendererSchedulerImpl::CreateMainThread() {
  return std::make_unique<WebThreadImplForRendererScheduler>(this);
}
