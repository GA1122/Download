void QuicTransportHost::OnRemoteStopped() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  stream_hosts_.clear();
  PostCrossThreadTask(
      *proxy_thread(), FROM_HERE,
      CrossThreadBind(&QuicTransportProxy::OnRemoteStopped, proxy_));
}
