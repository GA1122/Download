std::unique_ptr<ResourceHandler> DownloadResourceHandler::Create(
    net::URLRequest* request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::unique_ptr<ResourceHandler> handler(new DownloadResourceHandler(
      request, std::string(), download::DownloadSource::NAVIGATION, true));
  return handler;
}
