UtilityServiceFactory::CreateNetworkService() {
  return base::MakeUnique<NetworkServiceImpl>(std::move(network_registry_));
}
