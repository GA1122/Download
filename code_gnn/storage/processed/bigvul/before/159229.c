DownloadManagerImpl::UniqueUrlDownloadHandlerPtr BeginResourceDownload(
    std::unique_ptr<DownloadUrlParameters> params,
    std::unique_ptr<ResourceRequest> request,
    scoped_refptr<URLLoaderFactoryGetter> url_loader_factory_getter,
    scoped_refptr<storage::FileSystemContext> file_system_context,
    uint32_t download_id,
    base::WeakPtr<DownloadManagerImpl> download_manager) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (params->render_process_host_id() >= 0 &&
      !CanRequestURLFromRenderer(params->render_process_host_id(),
                                 params->url())) {
    CreateInterruptedDownload(params.get(),
                              DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST,
                              download_manager);
    return nullptr;
  }

  ResourceRequestInfo::WebContentsGetter getter =
      base::Bind(&GetWebContents, params->render_process_host_id(),
                 params->render_frame_host_routing_id(), -1);
  return DownloadManagerImpl::UniqueUrlDownloadHandlerPtr(
      ResourceDownloader::BeginDownload(
          download_manager, std::move(params), std::move(request),
          url_loader_factory_getter, file_system_context, getter, download_id,
          false)
          .release());
}
