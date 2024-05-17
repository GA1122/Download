void ExtensionDevToolsClientHost::DispatchOnInspectorFrontend(
    const std::string& message) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  if (profile == NULL || !profile->GetExtensionEventRouter())
    return;

  scoped_ptr<Value> result(base::JSONReader::Read(message, false));
  if (!result->IsType(Value::TYPE_DICTIONARY))
    return;
  DictionaryValue* dictionary = static_cast<DictionaryValue*>(result.get());

  int id;
  if (!dictionary->GetInteger("id", &id)) {
    std::string method_name;
    if (!dictionary->GetString("method", &method_name))
      return;

    ListValue args;
    args.Append(CreateDebuggeeId(tab_id_));
    args.Append(Value::CreateStringValue(method_name));
    Value* params_value;
    if (dictionary->Get("params", &params_value))
      args.Append(params_value->DeepCopy());

    std::string json_args;
    base::JSONWriter::Write(&args, false, &json_args);

    profile->GetExtensionEventRouter()->DispatchEventToExtension(
        extension_id_, keys::kOnEvent, json_args, profile, GURL());
  } else {
    SendCommandDebuggerFunction* function = pending_requests_[id];
    if (!function)
      return;

    function->SendResponseBody(dictionary);
    pending_requests_.erase(id);
  }
}
