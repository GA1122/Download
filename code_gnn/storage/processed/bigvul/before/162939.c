std::unique_ptr<service_manager::Service> ResourceCoordinatorService::Create() {
  auto resource_coordinator_service =
      std::make_unique<ResourceCoordinatorService>();

  return resource_coordinator_service;
}
