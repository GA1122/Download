int WebContext::BrowserContextDelegate::OnBeforeURLRequest(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    GURL* new_url) {
  QSharedPointer<WebContextProxyClient::IOClient> io_client = GetIOClient();
  if (!io_client) {
    return net::OK;
  }

  const content::ResourceRequestInfo* info =
      content::ResourceRequestInfo::ForRequest(request);
  if (!info) {
    return net::OK;
  }

  OxideQBeforeURLRequestEvent event(
      QUrl(QString::fromStdString(request->url().spec())),
      QString::fromStdString(request->method()),
      QString::fromStdString(request->referrer()),
      info->IsMainFrame());

  io_client->OnBeforeURLRequest(&event);

  OxideQBeforeURLRequestEventPrivate* eventp =
      OxideQBeforeURLRequestEventPrivate::get(&event);
  *new_url = GURL(eventp->new_url.toString().toStdString());

  return eventp->request_cancelled ? net::ERR_ABORTED : net::OK;
}
