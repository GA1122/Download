void RenderThreadImpl::SetUpEmbeddedWorkerChannelForServiceWorker(
    mojom::EmbeddedWorkerInstanceClientAssociatedRequest client_request) {
  EmbeddedWorkerInstanceClientImpl::Create(
      blink_initialized_time_, GetIOTaskRunner(), std::move(client_request));
}
