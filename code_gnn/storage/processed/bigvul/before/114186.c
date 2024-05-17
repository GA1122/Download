base::WaitableEvent* BrowserGpuChannelHostFactory::GetShutDownEvent() {
  return shutdown_event_.get();
}
