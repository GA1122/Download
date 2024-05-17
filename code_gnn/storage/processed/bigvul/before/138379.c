void ServiceManagerConnection::SetForProcess(
    std::unique_ptr<ServiceManagerConnection> connection) {
  DCHECK(!g_connection_for_process.Get());
  g_connection_for_process.Get() = std::move(connection);
}
