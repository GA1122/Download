bool WebGraphicsContext3DCommandBufferImpl::ShouldUseSwapClient() {
  GpuChannelHostFactory* factory = GpuChannelHostFactory::instance();
  return factory && factory->IsMainThread() && swap_client_.get();
}
