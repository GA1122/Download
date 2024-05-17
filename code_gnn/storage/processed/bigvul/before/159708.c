void DevToolsUIBindings::HandleMessageFromDevToolsFrontend(
    const std::string& message) {
  if (!frontend_host_)
    return;
  std::string method;
  base::ListValue empty_params;
  base::ListValue* params = &empty_params;

  base::DictionaryValue* dict = NULL;
  std::unique_ptr<base::Value> parsed_message = base::JSONReader::Read(message);
  if (!parsed_message ||
      !parsed_message->GetAsDictionary(&dict) ||
      !dict->GetString(kFrontendHostMethod, &method) ||
      (dict->HasKey(kFrontendHostParams) &&
          !dict->GetList(kFrontendHostParams, &params))) {
    LOG(ERROR) << "Invalid message was sent to embedder: " << message;
    return;
  }
  int id = 0;
  dict->GetInteger(kFrontendHostId, &id);
  embedder_message_dispatcher_->Dispatch(
      base::Bind(&DevToolsUIBindings::SendMessageAck,
                 weak_factory_.GetWeakPtr(),
                 id),
      method,
      params);
}
