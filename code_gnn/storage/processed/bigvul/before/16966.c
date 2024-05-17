int WebContext::BrowserContextDelegate::OnBeforeSendHeaders(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    net::HttpRequestHeaders* headers) {
  QSharedPointer<WebContextProxyClient::IOClient> io_client = GetIOClient();
  if (!io_client) {
    return net::OK;
  }

  const content::ResourceRequestInfo* info =
      content::ResourceRequestInfo::ForRequest(request);
  if (!info) {
    return net::OK;
  }

  OxideQBeforeSendHeadersEvent event(
      QUrl(QString::fromStdString(request->url().spec())),
      QString::fromStdString(request->method()),
      QString::fromStdString(request->referrer()),
      info->IsMainFrame());

  OxideQBeforeSendHeadersEventPrivate* eventp =
      OxideQBeforeSendHeadersEventPrivate::get(&event);
  eventp->headers = headers;

  io_client->OnBeforeSendHeaders(&event);

  return eventp->request_cancelled ? net::ERR_ABORTED : net::OK;
}
