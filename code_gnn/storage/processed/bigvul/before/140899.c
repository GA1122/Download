void PresentationConnection::bindProxy(
    std::unique_ptr<WebPresentationConnectionProxy> proxy) {
  DCHECK(proxy);
  m_proxy = std::move(proxy);
}
