void DownloadManagerImpl::BeginDownloadInternal(
    std::unique_ptr<download::DownloadUrlParameters> params,
    std::unique_ptr<storage::BlobDataHandle> blob_data_handle,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory,
    bool is_new_download,
    const GURL& site_url) {
  if (params->render_process_host_id() >= 0 &&
      !DownloadRequestUtils::IsURLSafe(params->render_process_host_id(),
                                       params->url())) {
    CreateInterruptedDownload(
        std::move(params),
        download::DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST,
        weak_factory_.GetWeakPtr());
    return;
  }

  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    if (!blob_url_loader_factory && params->url().SchemeIsBlob()) {
      blob_url_loader_factory =
          ChromeBlobStorageContext::URLLoaderFactoryForUrl(browser_context_,
                                                           params->url());
    }

    auto* rfh = RenderFrameHost::FromID(params->render_process_host_id(),
                                        params->render_frame_host_routing_id());
    bool content_initiated = params->content_initiated();
    if (rfh && content_initiated) {
      ResourceRequestInfo::WebContentsGetter web_contents_getter =
          base::BindRepeating(WebContents::FromFrameTreeNodeId,
                              rfh->GetFrameTreeNodeId());
      const GURL& url = params->url();
      const std::string& method = params->method();

      base::OnceCallback<void(bool  )>
          on_can_download_checks_done = base::BindOnce(
              &DownloadManagerImpl::BeginResourceDownloadOnChecksComplete,
              weak_factory_.GetWeakPtr(), std::move(params),
              std::move(blob_url_loader_factory), is_new_download, site_url);
      if (delegate_) {
        delegate_->CheckDownloadAllowed(std::move(web_contents_getter), url,
                                        method,
                                        std::move(on_can_download_checks_done));
        return;
      }
    }

    BeginResourceDownloadOnChecksComplete(
        std::move(params), std::move(blob_url_loader_factory), is_new_download,
        site_url, rfh ? !content_initiated : true);
  } else {
    StoragePartition* storage_partition =
        BrowserContext::GetStoragePartitionForSite(browser_context_, site_url);
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(
            &BeginDownload, std::move(params), std::move(blob_data_handle),
            browser_context_->GetResourceContext(),
            base::WrapRefCounted(storage_partition->GetURLRequestContext()),
            is_new_download, weak_factory_.GetWeakPtr()));
  }
}
