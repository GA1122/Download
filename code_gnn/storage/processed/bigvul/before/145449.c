void QuicStreamHost::Delete() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(transport_host_);
  transport_host_->OnRemoveStream(this);
}
