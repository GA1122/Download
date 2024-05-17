scoped_refptr<base::SingleThreadTaskRunner> QuicTransportHost::host_thread()
    const {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  return ice_transport_host_->host_thread();
}
