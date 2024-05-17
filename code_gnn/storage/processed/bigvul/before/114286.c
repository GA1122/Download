bool WebGraphicsContext3DCommandBufferImpl::ShouldUseSwapClient() {
  return factory_ && factory_->IsMainThread() && swap_client_.get();
}
