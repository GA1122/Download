std::unique_ptr<ResourceHandler> DownloadResourceHandler::CreateForNewRequest(
    net::URLRequest* request,
    const std::string& request_origin,
    download::DownloadSource download_source,
    bool follow_cross_origin_redirects) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::unique_ptr<ResourceHandler> handler(new DownloadResourceHandler(
      request, request_origin, download_source, follow_cross_origin_redirects));
  return handler;
}
