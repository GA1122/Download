bool RenderFrameImpl::CreatePlaceholderDocumentLoader(
    const blink::WebNavigationInfo& info) {
  auto navigation_params = blink::WebNavigationParams::CreateFromInfo(info);
  navigation_params->service_worker_network_provider =
      BuildServiceWorkerNetworkProviderForNavigation(
          nullptr  ,
          nullptr  );
  return frame_->CreatePlaceholderDocumentLoader(
      std::move(navigation_params), info.navigation_type, BuildDocumentState());
}
