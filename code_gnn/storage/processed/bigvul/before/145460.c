void QuicTransportHost::OnConnectionFailed(const std::string& error_details,
                                           bool from_remote) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  stream_hosts_.clear();
  PostCrossThreadTask(*proxy_thread(), FROM_HERE,
                      CrossThreadBind(&QuicTransportProxy::OnConnectionFailed,
                                      proxy_, error_details, from_remote));
}
