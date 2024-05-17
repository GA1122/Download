void DownloadManagerImpl::CreateDownloadHandlerForNavigation(
    base::WeakPtr<DownloadManagerImpl> download_manager,
    ResourceRequestInfo::WebContentsGetter web_contents_getter,
    std::unique_ptr<ResourceRequest> resource_request,
    std::vector<GURL> url_chain,
    scoped_refptr<ResourceResponse> response,
    net::CertStatus cert_status,
    mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  std::unique_ptr<ResourceDownloader> resource_downloader =
      ResourceDownloader::InterceptNavigationResponse(
          download_manager, std::move(resource_request),
          std::move(web_contents_getter), std::move(url_chain),
          std::move(response), std::move(cert_status),
          std::move(url_loader_client_endpoints));

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&DownloadManagerImpl::AddUrlDownloadHandler,
                     download_manager,
                     DownloadManagerImpl::UniqueUrlDownloadHandlerPtr(
                         resource_downloader.release())));
}
