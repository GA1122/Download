std::string WebContext::BrowserContextDelegate::GetUserAgentOverride(
    const GURL& url) {
  QSharedPointer<WebContextProxyClient::IOClient> io_client = GetIOClient();
  if (!io_client) {
    return std::string();
  }

  QString user_agent = io_client->GetUserAgentOverride(
      QUrl(QString::fromStdString(url.spec())));

  return user_agent.toStdString();
}
