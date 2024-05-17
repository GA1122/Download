void Core::AcceptBrokerClientInvitation(ConnectionParams connection_params) {
  RequestContext request_context;
  GetNodeController()->AcceptBrokerClientInvitation(
      std::move(connection_params));
}
