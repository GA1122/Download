void ExtensionDevToolsClientHost::SendMessageToBackend(
    DebuggerSendCommandFunction* function,
    const std::string& method,
    SendCommand::Params::CommandParams* command_params) {
  base::DictionaryValue protocol_request;
  int request_id = ++last_request_id_;
  pending_requests_[request_id] = function;
  protocol_request.SetInteger("id", request_id);
  protocol_request.SetString("method", method);
  if (command_params) {
    protocol_request.Set(
        "params", command_params->additional_properties.CreateDeepCopy());
  }

  std::string json_args;
  base::JSONWriter::Write(protocol_request, &json_args);
  agent_host_->DispatchProtocolMessage(this, json_args);
}
