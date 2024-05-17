void DownloadManagerImpl::InterceptNavigationOnChecksComplete(
    ResourceRequestInfo::WebContentsGetter web_contents_getter,
    std::unique_ptr<ResourceRequest> resource_request,
    std::vector<GURL> url_chain,
    scoped_refptr<ResourceResponse> response,
    net::CertStatus cert_status,
    mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    bool is_download_allowed) {
  if (!is_download_allowed)
    return;

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(&DownloadManagerImpl::CreateDownloadHandlerForNavigation,
                     weak_factory_.GetWeakPtr(), web_contents_getter,
                     std::move(resource_request), std::move(url_chain),
                     std::move(response), std::move(cert_status),
                     std::move(url_loader_client_endpoints)));
}
