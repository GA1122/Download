void TestingAutomationProvider::CreateNewAutomationProvider(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string channel_id;
  if (!args->GetString("channel_id", &channel_id)) {
    reply.SendError("'channel_id' missing or invalid");
    return;
  }

  AutomationProvider* provider = new TestingAutomationProvider(profile_);
  base::ThreadRestrictions::ScopedAllowIO allow_io;
  if (!provider->InitializeChannel(
          automation::kNamedInterfacePrefix + channel_id)) {
    reply.SendError("Failed to initialize channel: " + channel_id);
    return;
  }
  provider->SetExpectedTabCount(0);
  DCHECK(g_browser_process);
  g_browser_process->GetAutomationProviderList()->AddProvider(provider);
  reply.SendSuccess(NULL);
}
