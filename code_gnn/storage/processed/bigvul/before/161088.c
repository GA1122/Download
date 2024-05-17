void ServerStartedOnUI(base::WeakPtr<DevToolsHttpHandler> handler,
                       base::Thread* thread,
                       ServerWrapper* server_wrapper,
                       DevToolsSocketFactory* socket_factory,
                       std::unique_ptr<net::IPEndPoint> ip_address) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (handler && thread && server_wrapper) {
    handler->ServerStarted(
        std::unique_ptr<base::Thread>(thread),
        std::unique_ptr<ServerWrapper>(server_wrapper),
        std::unique_ptr<DevToolsSocketFactory>(socket_factory),
        std::move(ip_address));
  } else {
    TerminateOnUI(std::unique_ptr<base::Thread>(thread),
                  std::unique_ptr<ServerWrapper>(server_wrapper),
                  std::unique_ptr<DevToolsSocketFactory>(socket_factory));
  }
}
