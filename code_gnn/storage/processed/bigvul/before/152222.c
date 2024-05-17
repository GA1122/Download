void RenderFrameImpl::CommitSyncNavigation(
    std::unique_ptr<blink::WebNavigationInfo> info) {
  auto navigation_params = WebNavigationParams::CreateFromInfo(*info);
  navigation_params->service_worker_network_provider =
      ServiceWorkerNetworkProviderForFrame::CreateInvalidInstance();
  frame_->CommitNavigation(std::move(navigation_params), BuildDocumentState());
}
