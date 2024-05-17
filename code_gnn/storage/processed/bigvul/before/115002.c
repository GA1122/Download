void TestingAutomationProvider::PerformActionOnSearchEngine(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  TemplateURLService* url_model =
      TemplateURLServiceFactory::GetForProfile(browser->profile());
  std::string keyword;
  std::string action;
  if (!args->GetString("keyword", &keyword) ||
      !args->GetString("action", &action)) {
    AutomationJSONReply(this, reply_message).SendError(
        "One or more inputs invalid");
    return;
  }
  const TemplateURL* template_url(
      url_model->GetTemplateURLForKeyword(UTF8ToUTF16(keyword)));
  if (template_url == NULL) {
    AutomationJSONReply(this, reply_message)
        .SendError(StringPrintf("No match for keyword: %s", keyword.c_str()));
    return;
  }
  if (action == "delete") {
    url_model->AddObserver(new AutomationProviderSearchEngineObserver(
      this, reply_message));
    url_model->Remove(template_url);
  } else if (action == "default") {
    url_model->AddObserver(new AutomationProviderSearchEngineObserver(
      this, reply_message));
    url_model->SetDefaultSearchProvider(template_url);
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError(StringPrintf("Invalid action: %s", action.c_str()));
  }
}
