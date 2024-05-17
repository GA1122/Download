void TestingAutomationProvider::LoadSearchEngineInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  TemplateURLService* url_model =
      TemplateURLServiceFactory::GetForProfile(browser->profile());
  if (url_model->loaded()) {
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
    return;
  }
  url_model->AddObserver(new AutomationProviderSearchEngineObserver(
      this, reply_message));
  url_model->Load();
}
