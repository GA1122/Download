int ChromeNetworkDelegate::OnBeforeSocketStreamConnect(
    net::SocketStream* socket,
    const net::CompletionCallback& callback) {
#if defined(ENABLE_CONFIGURATION_POLICY)
  if (url_blacklist_manager_ &&
      url_blacklist_manager_->IsURLBlocked(socket->url())) {
    socket->net_log()->AddEvent(
        net::NetLog::TYPE_CHROME_POLICY_ABORTED_REQUEST,
        net::NetLog::StringCallback("url",
                                    &socket->url().possibly_invalid_spec()));
    return net::ERR_BLOCKED_BY_ADMINISTRATOR;
  }
#endif
  return net::OK;
}
