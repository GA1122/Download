void RenderThreadImpl::OnRendererVisible() {
  blink::MainThreadIsolate()->IsolateInForegroundNotification();
  if (!GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden())
    return;
  renderer_scheduler_->SetRendererHidden(false);
  ScheduleIdleHandler(kLongIdleHandlerDelayMs);
}
