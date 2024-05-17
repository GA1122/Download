void ParseConfigurationArguments(const base::ListValue* args,
                                 SyncConfigInfo* config,
                                 const base::Value** callback_id) {
  std::string json;
  if (args->Get(0, callback_id) && args->GetString(1, &json) && !json.empty())
    CHECK(GetConfiguration(json, config));
  else
    NOTREACHED();
}
