void TestingAutomationProvider::SetLocalStatePrefs(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  std::string path;
  Value* val;
  AutomationJSONReply reply(this, reply_message);
  if (args->GetString("path", &path) && args->Get("value", &val)) {
    PrefService* pref_service = g_browser_process->local_state();
    const PrefService::Preference* pref =
        pref_service->FindPreference(path.c_str());
    if (!pref) {   
      reply.SendError("pref not registered.");
      return;
    } else if (pref->IsManaged()) {   
      reply.SendError("pref is managed. cannot be changed.");
      return;
    } else {   
      pref_service->Set(path.c_str(), *val);
    }
  } else {
    reply.SendError("no pref path or value given.");
    return;
  }

  reply.SendSuccess(NULL);
}
