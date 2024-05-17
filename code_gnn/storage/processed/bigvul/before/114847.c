void TestingAutomationProvider::AddOrEditSearchEngine(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  TemplateURLService* url_model =
      TemplateURLServiceFactory::GetForProfile(browser->profile());
  const TemplateURL* template_url;
  string16 new_title;
  string16 new_keyword;
  std::string new_url;
  std::string keyword;
  if (!args->GetString("new_title", &new_title) ||
      !args->GetString("new_keyword", &new_keyword) ||
      !args->GetString("new_url", &new_url)) {
    AutomationJSONReply(this, reply_message)
        .SendError("One or more inputs invalid");
    return;
  }
  std::string new_ref_url = TemplateURLRef::DisplayURLToURLRef(
      UTF8ToUTF16(new_url));
  scoped_ptr<KeywordEditorController> controller(
      new KeywordEditorController(browser->profile()));
  if (args->GetString("keyword", &keyword)) {
    template_url = url_model->GetTemplateURLForKeyword(UTF8ToUTF16(keyword));
    if (template_url == NULL) {
      AutomationJSONReply(this, reply_message)
          .SendError(StringPrintf("No match for keyword: %s", keyword.c_str()));
      return;
    }
    url_model->AddObserver(new AutomationProviderSearchEngineObserver(
        this, reply_message));
    controller->ModifyTemplateURL(template_url, new_title, new_keyword,
                                  new_ref_url);
  } else {
    url_model->AddObserver(new AutomationProviderSearchEngineObserver(
        this, reply_message));
    controller->AddTemplateURL(new_title, new_keyword, new_ref_url);
  }
}
