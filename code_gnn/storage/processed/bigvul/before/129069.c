void ExtensionDevToolsClientHost::DispatchOnInspectorFrontend(
    const std::string& message) {
  if (!EventRouter::Get(profile_))
    return;

  scoped_ptr<base::Value> result(base::JSONReader::Read(message));
  if (!result->IsType(base::Value::TYPE_DICTIONARY))
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

    scoped_ptr<base::ListValue> args(
        OnEvent::Create(debuggee_, method_name, params));
    scoped_ptr<Event> event(new Event(OnEvent::kEventName, args.Pass()));
    event->restrict_to_browser_context = profile_;
    EventRouter::Get(profile_)
        ->DispatchEventToExtension(extension_id_, event.Pass());
  } else {
    DebuggerSendCommandFunction* function = pending_requests_[id].get();
    if (!function)
      return;

    function->SendResponseBody(dictionary);
    pending_requests_.erase(id);
  }
}
