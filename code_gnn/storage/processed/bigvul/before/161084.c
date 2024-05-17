void ServerWrapper::SendOverWebSocket(int connection_id,
                                      const std::string& message) {
  server_->SendOverWebSocket(connection_id, message,
                             kDevtoolsHttpHandlerTrafficAnnotation);
}
