void TestingAutomationProvider::GetPluginsInfoCallback(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message,
    const std::vector<webkit::WebPluginInfo>& plugins) {
  PluginPrefs* plugin_prefs = PluginPrefs::GetForProfile(browser->profile());
  ListValue* items = new ListValue;
  for (std::vector<webkit::WebPluginInfo>::const_iterator it =
           plugins.begin();
       it != plugins.end();
       ++it) {
    DictionaryValue* item = new DictionaryValue;
    item->SetString("name", it->name);
    item->SetString("path", it->path.value());
    item->SetString("version", it->version);
    item->SetString("desc", it->desc);
    item->SetBoolean("enabled", plugin_prefs->IsPluginEnabled(*it));
    ListValue* mime_types = new ListValue();
    for (std::vector<webkit::WebPluginMimeType>::const_iterator type_it =
             it->mime_types.begin();
         type_it != it->mime_types.end();
         ++type_it) {
      DictionaryValue* mime_type = new DictionaryValue();
      mime_type->SetString("mimeType", type_it->mime_type);
      mime_type->SetString("description", type_it->description);

      ListValue* file_extensions = new ListValue();
      for (std::vector<std::string>::const_iterator ext_it =
               type_it->file_extensions.begin();
           ext_it != type_it->file_extensions.end();
           ++ext_it) {
        file_extensions->Append(new StringValue(*ext_it));
      }
      mime_type->Set("fileExtensions", file_extensions);

      mime_types->Append(mime_type);
    }
    item->Set("mimeTypes", mime_types);
    items->Append(item);
  }
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->Set("plugins", items);   

  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
