void DownloadManagerImpl::InterceptNavigation(
    std::unique_ptr<network::ResourceRequest> resource_request,
    std::vector<GURL> url_chain,
    scoped_refptr<network::ResourceResponse> response,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    net::CertStatus cert_status,
    int frame_tree_node_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!delegate_) {
    DropDownload();
    return;
  }

  const GURL& url = resource_request->url;
  const std::string& method = resource_request->method;

  ResourceRequestInfo::WebContentsGetter web_contents_getter =
      base::BindRepeating(WebContents::FromFrameTreeNodeId, frame_tree_node_id);

  base::OnceCallback<void(bool  )>
      on_download_checks_done = base::BindOnce(
          &DownloadManagerImpl::InterceptNavigationOnChecksComplete,
          weak_factory_.GetWeakPtr(), web_contents_getter,
          std::move(resource_request), std::move(url_chain),
          std::move(response), cert_status,
          std::move(url_loader_client_endpoints));

  delegate_->CheckDownloadAllowed(std::move(web_contents_getter), url, method,
                                  std::move(on_download_checks_done));
}
