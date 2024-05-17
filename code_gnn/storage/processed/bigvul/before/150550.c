std::string DataReductionProxyConfig::GetCurrentNetworkID() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return DoGetCurrentNetworkID(network_connection_tracker_);
}
