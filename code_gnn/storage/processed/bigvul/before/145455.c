scoped_refptr<base::SingleThreadTaskRunner> QuicStreamHost::proxy_thread()
    const {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(transport_host_);
  return transport_host_->proxy_thread();
}
