void DownloadManagerImpl::BeginDownloadInternal(
    std::unique_ptr<content::DownloadUrlParameters> params,
    uint32_t id) {
  download::InProgressCache* in_progress_cache =
      GetBrowserContext()->GetDownloadManagerDelegate()->GetInProgressCache();
  if (in_progress_cache) {
    in_progress_cache->AddOrReplaceEntry(
        download::DownloadEntry(params->guid(), params->request_origin(),
                                ToDownloadSource(params->download_source())));
  }

  if (base::FeatureList::IsEnabled(features::kNetworkService)) {
    std::unique_ptr<ResourceRequest> request = CreateResourceRequest(
        params.get());
    StoragePartitionImpl* storage_partition =
        GetStoragePartition(browser_context_, params->render_process_host_id(),
                            params->render_frame_host_routing_id());
    BrowserThread::PostTaskAndReplyWithResult(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(
            &BeginResourceDownload, std::move(params), std::move(request),
            storage_partition->url_loader_factory_getter(),
             base::WrapRefCounted(storage_partition->GetFileSystemContext()),
             id, weak_factory_.GetWeakPtr()),
        base::BindOnce(&DownloadManagerImpl::AddUrlDownloadHandler,
                       weak_factory_.GetWeakPtr()));
  } else {
    BrowserThread::PostTaskAndReplyWithResult(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(&BeginDownload, std::move(params),
                       browser_context_->GetResourceContext(), id,
                       weak_factory_.GetWeakPtr()),
         base::BindOnce(&DownloadManagerImpl::AddUrlDownloadHandler,
                        weak_factory_.GetWeakPtr()));
   }
}
