void DevToolsHttpHandler::ServerStarted(
    std::unique_ptr<base::Thread> thread,
    std::unique_ptr<ServerWrapper> server_wrapper,
    std::unique_ptr<DevToolsSocketFactory> socket_factory,
    std::unique_ptr<net::IPEndPoint> ip_address) {
  thread_ = std::move(thread);
  server_wrapper_ = std::move(server_wrapper);
  socket_factory_ = std::move(socket_factory);
  server_ip_address_ = std::move(ip_address);
}
