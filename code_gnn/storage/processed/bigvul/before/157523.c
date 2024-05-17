std::string TestDataReductionProxyConfig::GetCurrentNetworkID() const {
  if (current_network_id_) {
    return current_network_id_.value();
  }
  return DataReductionProxyConfig::GetCurrentNetworkID();
}
