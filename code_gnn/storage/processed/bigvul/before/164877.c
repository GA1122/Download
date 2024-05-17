void DownloadManagerImpl::BeginResourceDownloadOnChecksComplete(
    std::unique_ptr<download::DownloadUrlParameters> params,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory,
    bool is_new_download,
    const GURL& site_url,
    bool is_download_allowed) {
  if (!is_download_allowed) {
    DropDownload();
    return;
  }

  GURL tab_url, tab_referrer_url;
  auto* rfh = RenderFrameHost::FromID(params->render_process_host_id(),
                                      params->render_frame_host_routing_id());
  if (rfh) {
    auto* web_contents = WebContents::FromRenderFrameHost(rfh);
    NavigationEntry* entry = web_contents->GetController().GetVisibleEntry();
    if (entry) {
      tab_url = entry->GetURL();
      tab_referrer_url = entry->GetReferrer().url;
    }
  }

  DCHECK_EQ(params->url().SchemeIsBlob(), bool{blob_url_loader_factory});
  scoped_refptr<download::DownloadURLLoaderFactoryGetter>
      url_loader_factory_getter;
  if (blob_url_loader_factory) {
    DCHECK(params->url().SchemeIsBlob());
    url_loader_factory_getter =
        base::MakeRefCounted<download::DownloadURLLoaderFactoryGetterImpl>(
            blob_url_loader_factory->Clone());
  } else if (params->url().SchemeIsFile()) {
    url_loader_factory_getter =
        base::MakeRefCounted<FileDownloadURLLoaderFactoryGetter>(
            params->url(), browser_context_->GetPath(),
            browser_context_->GetSharedCorsOriginAccessList());
  } else if (params->url().SchemeIs(content::kChromeUIScheme)) {
    url_loader_factory_getter =
        base::MakeRefCounted<WebUIDownloadURLLoaderFactoryGetter>(
            rfh, params->url());
  } else if (rfh && params->url().SchemeIsFileSystem()) {
    StoragePartitionImpl* storage_partition =
        static_cast<StoragePartitionImpl*>(
            BrowserContext::GetStoragePartitionForSite(browser_context_,
                                                       site_url));
    std::string storage_domain;
    auto* site_instance = rfh->GetSiteInstance();
    if (site_instance) {
      std::string partition_name;
      bool in_memory;
      GetContentClient()->browser()->GetStoragePartitionConfigForSite(
          browser_context_, site_url, true, &storage_domain, &partition_name,
          &in_memory);
    }
    url_loader_factory_getter =
        base::MakeRefCounted<FileSystemDownloadURLLoaderFactoryGetter>(
            params->url(), rfh,  false,
            storage_partition->GetFileSystemContext(), storage_domain);
  } else {
    StoragePartitionImpl* storage_partition =
        static_cast<StoragePartitionImpl*>(
            BrowserContext::GetStoragePartitionForSite(browser_context_,
                                                       site_url));
    url_loader_factory_getter =
        CreateDownloadURLLoaderFactoryGetter(storage_partition, rfh, true);
  }

  in_progress_manager_->BeginDownload(
      std::move(params), std::move(url_loader_factory_getter), is_new_download,
      site_url, tab_url, tab_referrer_url);
}
