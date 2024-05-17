void DevToolsSession::DispatchProtocolMessage(const std::string& message) {
  std::unique_ptr<base::Value> value = base::JSONReader::Read(message);

  DevToolsManagerDelegate* delegate =
      DevToolsManager::GetInstance()->delegate();
  if (value && value->is_dict() && delegate) {
    base::DictionaryValue* dict_value =
        static_cast<base::DictionaryValue*>(value.get());

    if (delegate->HandleCommand(agent_host_, client_, dict_value))
      return;

    if (delegate->HandleAsyncCommand(agent_host_, client_, dict_value,
                                     base::Bind(&DevToolsSession::SendResponse,
                                                weak_factory_.GetWeakPtr()))) {
      return;
    }
  }

  int call_id;
  std::string method;
  if (dispatcher_->dispatch(protocol::toProtocolValue(value.get(), 1000),
                            &call_id,
                            &method) != protocol::Response::kFallThrough) {
    return;
  }

  DCHECK(!browser_only_);

  if (suspended_sending_messages_to_agent_) {
    suspended_messages_.push_back({call_id, method, message});
    return;
  }

  DispatchProtocolMessageToAgent(call_id, method, message);
  waiting_for_response_messages_[call_id] = {method, message};
}
