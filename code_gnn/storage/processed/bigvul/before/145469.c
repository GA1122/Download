QuicTransportHost::~QuicTransportHost() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  if (ice_transport_host_) {
    ice_transport_host_->DisconnectConsumer(this);
  }
}
