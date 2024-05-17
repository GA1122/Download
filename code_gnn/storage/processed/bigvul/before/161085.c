void ServerWrapper::SendResponse(int connection_id,
                                 const net::HttpServerResponseInfo& response) {
  server_->SendResponse(connection_id, response,
                        kDevtoolsHttpHandlerTrafficAnnotation);
}
