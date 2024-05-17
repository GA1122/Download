void QuicTransportHost::OnRemoveStream(QuicStreamHost* stream_host_to_remove) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  auto it = stream_hosts_.find(stream_host_to_remove);
  DCHECK(it != stream_hosts_.end());
  stream_hosts_.erase(it);
}
