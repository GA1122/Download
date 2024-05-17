void QuicStreamHost::OnRemoteFinish() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  PostCrossThreadTask(
      *proxy_thread(), FROM_HERE,
      CrossThreadBind(&QuicStreamProxy::OnRemoteFinish, stream_proxy_));
  readable_ = false;
  if (!readable_ && !writeable_) {
    Delete();
  }
}
