void WebContext::init(
    const QWeakPointer<WebContextProxyClient::IOClient>& io_client) {
  delegate_->Init(io_client);
}
