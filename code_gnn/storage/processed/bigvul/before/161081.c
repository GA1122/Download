void ServerWrapper::Send500(int connection_id,
                            const std::string& message) {
  server_->Send500(connection_id, message,
                   kDevtoolsHttpHandlerTrafficAnnotation);
}
