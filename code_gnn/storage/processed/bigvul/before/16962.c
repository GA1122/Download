void WebContext::BrowserContextDelegate::Init(
    const QWeakPointer<WebContextProxyClient::IOClient>& io_client) {
  base::AutoLock lock(io_client_lock_);
  io_client_ = io_client;
}
