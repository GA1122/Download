void RenderThreadImpl::OnRendererHidden() {
  blink::MainThreadIsolate()->IsolateInBackgroundNotification();
  if (!GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden())
    return;
  renderer_scheduler_->SetRendererHidden(true);
  ScheduleIdleHandler(kInitialIdleHandlerDelayMs);
}
