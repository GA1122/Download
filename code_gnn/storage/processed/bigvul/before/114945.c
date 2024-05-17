void TestingAutomationProvider::GetSearchEngineInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  TemplateURLService* url_model =
      TemplateURLServiceFactory::GetForProfile(browser->profile());
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  ListValue* search_engines = new ListValue;
  std::vector<const TemplateURL*> template_urls = url_model->GetTemplateURLs();
  for (std::vector<const TemplateURL*>::const_iterator it =
       template_urls.begin(); it != template_urls.end(); ++it) {
    DictionaryValue* search_engine = new DictionaryValue;
    search_engine->SetString("short_name", UTF16ToUTF8((*it)->short_name()));
    search_engine->SetString("description", UTF16ToUTF8((*it)->description()));
    search_engine->SetString("keyword", UTF16ToUTF8((*it)->keyword()));
    search_engine->SetBoolean("in_default_list", (*it)->ShowInDefaultList());
    search_engine->SetBoolean("is_default",
        (*it) == url_model->GetDefaultSearchProvider());
    search_engine->SetBoolean("is_valid", (*it)->url()->IsValid());
    search_engine->SetBoolean("supports_replacement",
                              (*it)->url()->SupportsReplacement());
    search_engine->SetString("url", (*it)->url()->url());
    search_engine->SetString("host", (*it)->url()->GetHost());
    search_engine->SetString("path", (*it)->url()->GetPath());
    search_engine->SetString("display_url",
                             UTF16ToUTF8((*it)->url()->DisplayURL()));
    search_engines->Append(search_engine);
  }
  return_value->Set("search_engines", search_engines);
  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
