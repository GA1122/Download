RenderProcessHostImpl::GetProcessResourceCoordinator() {
  if (process_resource_coordinator_)
    return process_resource_coordinator_.get();

  if (!resource_coordinator::IsResourceCoordinatorEnabled()) {
    process_resource_coordinator_ =
        std::make_unique<resource_coordinator::ProcessResourceCoordinator>(
            nullptr);
  } else {
    auto* connection = ServiceManagerConnection::GetForProcess();
    process_resource_coordinator_ =
        std::make_unique<resource_coordinator::ProcessResourceCoordinator>(
            connection ? connection->GetConnector() : nullptr);
  }
  return process_resource_coordinator_.get();
}
