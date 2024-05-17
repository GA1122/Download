void ServerWrapper::Send404(int connection_id) {
  server_->Send404(connection_id, kDevtoolsHttpHandlerTrafficAnnotation);
}
