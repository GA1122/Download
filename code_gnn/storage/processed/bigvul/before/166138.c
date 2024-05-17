service_manager::Connector* MaybeGetFrameResourceCoordinator() {
  auto* connection = ServiceManagerConnection::GetForProcess();
  if (!connection)
    return nullptr;

  return connection->GetConnector();
}
