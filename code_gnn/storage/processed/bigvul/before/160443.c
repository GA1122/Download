void GetRestrictedCookieManager(
    RenderFrameHostImpl* render_frame_host_impl,
    network::mojom::RestrictedCookieManagerRequest request) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableExperimentalWebPlatformFeatures)) {
    return;
  }

  BrowserContext* browser_context =
      render_frame_host_impl->GetProcess()->GetBrowserContext();
  StoragePartition* storage_partition =
      BrowserContext::GetDefaultStoragePartition(browser_context);
  network::mojom::NetworkContext* network_context =
      storage_partition->GetNetworkContext();
  uint32_t render_process_id = render_frame_host_impl->GetProcess()->GetID();
  uint32_t render_frame_id = render_frame_host_impl->GetRoutingID();
  network_context->GetRestrictedCookieManager(
      std::move(request), render_process_id, render_frame_id);
}
