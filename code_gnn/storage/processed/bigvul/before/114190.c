BrowserGpuChannelHostFactory::~BrowserGpuChannelHostFactory() {
  shutdown_event_->Signal();
}
