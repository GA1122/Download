void BrowserGpuChannelHostFactorySetApplicationVisible(bool is_visible) {
  DCHECK_EQ(BrowserMainLoop::GetInstance()->gpu_channel_establish_factory(),
            BrowserGpuChannelHostFactory::instance());
  BrowserGpuChannelHostFactory::instance()->SetApplicationVisible(is_visible);
}
