void DownloadManagerImpl::InterceptNavigationOnChecksComplete(
    ResourceRequestInfo::WebContentsGetter web_contents_getter,
    std::unique_ptr<network::ResourceRequest> resource_request,
    std::vector<GURL> url_chain,
    scoped_refptr<network::ResourceResponse> response,
    net::CertStatus cert_status,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    bool is_download_allowed) {
  if (!is_download_allowed) {
    DropDownload();
    return;
  }

  int render_process_id = -1;
  int render_frame_id = -1;
  GURL site_url, tab_url, tab_referrer_url;
  RenderFrameHost* render_frame_host = nullptr;
  WebContents* web_contents = std::move(web_contents_getter).Run();
  if (web_contents) {
    render_frame_host = web_contents->GetMainFrame();
    if (render_frame_host) {
      render_process_id = render_frame_host->GetProcess()->GetID();
      render_frame_id = render_frame_host->GetRoutingID();
    }
    NavigationEntry* entry = web_contents->GetController().GetVisibleEntry();
    if (entry) {
      tab_url = entry->GetURL();
      tab_referrer_url = entry->GetReferrer().url;
    }
  }
  StoragePartitionImpl* storage_partition =
      GetStoragePartition(browser_context_, render_process_id, render_frame_id);
  in_progress_manager_->InterceptDownloadFromNavigation(
      std::move(resource_request), render_process_id, render_frame_id, site_url,
      tab_url, tab_referrer_url, std::move(url_chain), std::move(response),
      std::move(cert_status), std::move(url_loader_client_endpoints),
      CreateDownloadURLLoaderFactoryGetter(storage_partition, render_frame_host,
                                           false));
}
