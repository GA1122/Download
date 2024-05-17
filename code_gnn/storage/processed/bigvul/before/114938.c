void TestingAutomationProvider::GetPluginsInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  PluginService::GetInstance()->GetPlugins(
      base::Bind(&TestingAutomationProvider::GetPluginsInfoCallback,
          this, browser, args, reply_message));
}
