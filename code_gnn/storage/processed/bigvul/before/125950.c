void AutomationProviderHistoryObserver::HistoryQueryComplete(
    HistoryService::Handle request_handle,
    history::QueryResults* results) {
  if (!provider_) {
    delete this;
    return;
  }

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

  ListValue* history_list = new ListValue;
  for (size_t i = 0; i < results->size(); ++i) {
    DictionaryValue* page_value = new DictionaryValue;
    history::URLResult const &page = (*results)[i];
    page_value->SetString("title", page.title());
    page_value->SetString("url", page.url().spec());
    page_value->SetDouble("time",
                          static_cast<double>(page.visit_time().ToDoubleT()));
    page_value->SetString("snippet", page.snippet().text());
    page_value->SetBoolean(
        "starred",
        BookmarkModelFactory::GetForProfile(
            provider_->profile())->IsBookmarked(page.url()));
    history_list->Append(page_value);
  }

  return_value->Set("history", history_list);
  AutomationJSONReply reply(provider_, reply_message_.release());
  reply.SendSuccess(return_value.get());
  delete this;
}
