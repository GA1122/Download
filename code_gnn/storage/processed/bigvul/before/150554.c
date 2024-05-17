DataReductionProxyConfig::GetNetworkPropertiesManager() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return *network_properties_manager_;
}
