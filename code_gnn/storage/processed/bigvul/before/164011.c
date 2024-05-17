void BeginDownload(
    std::unique_ptr<download::DownloadUrlParameters> params,
    std::unique_ptr<storage::BlobDataHandle> blob_data_handle,
    content::ResourceContext* resource_context,
    scoped_refptr<net::URLRequestContextGetter> url_request_context_getter,
    bool is_new_download,
    base::WeakPtr<DownloadManagerImpl> download_manager) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  download::UrlDownloadHandler::UniqueUrlDownloadHandlerPtr downloader(
      nullptr, base::OnTaskRunnerDeleter(base::ThreadTaskRunnerHandle::Get()));

  params->set_blob_storage_context_getter(
      base::BindOnce(&BlobStorageContextGetter, resource_context));
  std::unique_ptr<net::URLRequest> url_request =
      DownloadRequestCore::CreateRequestOnIOThread(
          is_new_download, params.get(), std::move(url_request_context_getter));
  if (blob_data_handle) {
    storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
        url_request.get(), std::move(blob_data_handle));
  }

  if (params->render_process_host_id() >= 0) {
    download::DownloadInterruptReason reason =
        DownloadManagerImpl::BeginDownloadRequest(
            std::move(url_request), resource_context, params.get());

    if (reason != download::DOWNLOAD_INTERRUPT_REASON_NONE) {
      CreateInterruptedDownload(std::move(params), reason, download_manager);
      return;
    }
  } else {
    downloader.reset(UrlDownloader::BeginDownload(download_manager,
                                                  std::move(url_request),
                                                  params.get(), false)
                         .release());
  }
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(
          &download::UrlDownloadHandler::Delegate::OnUrlDownloadHandlerCreated,
          download_manager, std::move(downloader)));
}
