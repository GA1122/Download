JsArgList SyncManager::SyncInternal::GetClientServerTraffic(
    const JsArgList& args) {
  ListValue return_args;
  ListValue* value = traffic_recorder_.ToValue();
  if (value != NULL)
    return_args.Append(value);
  return JsArgList(&return_args);
}
