void ServerWrapper::AcceptWebSocket(int connection_id,
                                    const net::HttpServerRequestInfo& request) {
  server_->SetSendBufferSize(connection_id, kSendBufferSizeForDevTools);
  server_->SetReceiveBufferSize(connection_id, kReceiveBufferSizeForDevTools);
  server_->AcceptWebSocket(connection_id, request,
                           kDevtoolsHttpHandlerTrafficAnnotation);
}
