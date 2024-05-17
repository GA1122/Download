void TestingAutomationProvider::GetHistoryInfo(Browser* browser,
                                               DictionaryValue* args,
                                               IPC::Message* reply_message) {
  consumer_.CancelAllRequests();

  string16 search_text;
  args->GetString("search_text", &search_text);

  HistoryService* hs = browser->profile()->GetHistoryService(
      Profile::EXPLICIT_ACCESS);
  history::QueryOptions options;
  AutomationProviderHistoryObserver* history_observer =
      new AutomationProviderHistoryObserver(this, reply_message);
  hs->QueryHistory(
      search_text,
      options,
      &consumer_,
      base::Bind(&AutomationProviderHistoryObserver::HistoryQueryComplete,
                 base::Unretained(history_observer)));
}
