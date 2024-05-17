void Core::SendBrokerClientInvitation(
    base::ProcessHandle target_process,
    ConnectionParams connection_params,
    const std::vector<std::pair<std::string, ports::PortRef>>& attached_ports,
    const ProcessErrorCallback& process_error_callback) {
  RequestContext request_context;
  GetNodeController()->SendBrokerClientInvitation(
      target_process, std::move(connection_params), attached_ports,
      process_error_callback);
}
