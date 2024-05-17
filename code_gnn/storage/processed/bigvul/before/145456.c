void QuicStreamHost::set_proxy(base::WeakPtr<QuicStreamProxy> stream_proxy) {
  DETACH_FROM_THREAD(thread_checker_);
  stream_proxy_ = stream_proxy;
}
