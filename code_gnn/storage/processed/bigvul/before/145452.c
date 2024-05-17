void QuicStreamHost::OnRemoteReset() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  PostCrossThreadTask(
      *proxy_thread(), FROM_HERE,
      CrossThreadBind(&QuicStreamProxy::OnRemoteReset, stream_proxy_));
  Delete();
}
