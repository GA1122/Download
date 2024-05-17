void TestingAutomationProvider::SetPrefs(DictionaryValue* args,
                                         IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  Browser* browser;
  std::string error_msg;
  if (!GetBrowserFromJSONArgs(args, &browser, &error_msg)) {
    reply.SendError(error_msg);
    return;
  }
  std::string path;
  Value* val;
  if (args->GetString("path", &path) && args->Get("value", &val)) {
    PrefService* pref_service = browser->profile()->GetPrefs();
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
