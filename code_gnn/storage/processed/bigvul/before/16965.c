int WebContext::BrowserContextDelegate::OnBeforeRedirect(
    net::URLRequest* request,
    const GURL& new_location) {
  QSharedPointer<WebContextProxyClient::IOClient> io_client = GetIOClient();
  if (!io_client) {
    return net::OK;
  }

  const content::ResourceRequestInfo* info =
      content::ResourceRequestInfo::ForRequest(request);
  if (!info) {
    return net::OK;
  }

  OxideQBeforeRedirectEvent event(
      QUrl(QString::fromStdString(new_location.spec())),
      QString::fromStdString(request->method()),
      QString::fromStdString(request->referrer()),
      info->IsMainFrame(),
      QUrl(QString::fromStdString(request->original_url().spec())));

  io_client->OnBeforeRedirect(&event);

  OxideQBeforeRedirectEventPrivate* eventp =
      OxideQBeforeRedirectEventPrivate::get(&event);

  return eventp->request_cancelled ? net::ERR_ABORTED : net::OK;
}
