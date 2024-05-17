network::mojom::NetworkContext* PreconnectManager::GetNetworkContext() const {
  if (network_context_)
    return network_context_;

  if (profile_->AsTestingProfile()) {
    return nullptr;
  }

  return content::BrowserContext::GetDefaultStoragePartition(profile_)
      ->GetNetworkContext();
}
