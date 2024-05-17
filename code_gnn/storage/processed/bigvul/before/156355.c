void ExtensionDevToolsClientHost::DispatchProtocolMessage(
    DevToolsAgentHost* agent_host, const std::string& message) {
  DCHECK(agent_host == agent_host_.get());
  if (!EventRouter::Get(profile_))
    return;

  std::unique_ptr<base::Value> result = base::JSONReader::Read(message);
  if (!result || !result->is_dict())
    return;
  base::DictionaryValue* dictionary =
      static_cast<base::DictionaryValue*>(result.get());

  int id;
  if (!dictionary->GetInteger("id", &id)) {
    std::string method_name;
    if (!dictionary->GetString("method", &method_name))
      return;

    OnEvent::Params params;
    base::DictionaryValue* params_value;
    if (dictionary->GetDictionary("params", &params_value))
      params.additional_properties.Swap(params_value);

    std::unique_ptr<base::ListValue> args(
        OnEvent::Create(debuggee_, method_name, params));
    auto event =
        std::make_unique<Event>(events::DEBUGGER_ON_EVENT, OnEvent::kEventName,
                                std::move(args), profile_);
    EventRouter::Get(profile_)->DispatchEventToExtension(extension_id(),
                                                         std::move(event));
  } else {
    DebuggerSendCommandFunction* function = pending_requests_[id].get();
    if (!function)
      return;

    function->SendResponseBody(dictionary);
    pending_requests_.erase(id);
  }
}
