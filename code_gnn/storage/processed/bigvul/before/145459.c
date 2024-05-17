void QuicTransportHost::OnConnected() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  PostCrossThreadTask(
      *proxy_thread(), FROM_HERE,
      CrossThreadBind(&QuicTransportProxy::OnConnected, proxy_));
}
