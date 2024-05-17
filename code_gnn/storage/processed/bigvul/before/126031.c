void NTPInfoObserver::OnTopSitesReceived(
    const history::MostVisitedURLList& visited_list) {
  if (!automation_) {
    delete this;
    return;
  }

  ListValue* list_value = new ListValue;
  for (size_t i = 0; i < visited_list.size(); ++i) {
    const history::MostVisitedURL& visited = visited_list[i];
    if (visited.url.spec().empty())
      break;   
    DictionaryValue* dict = new DictionaryValue;
    dict->SetString("url", visited.url.spec());
    dict->SetString("title", visited.title);
    list_value->Append(dict);
  }
  ntp_info_->Set("most_visited", list_value);
  AutomationJSONReply(automation_,
                      reply_message_.release()).SendSuccess(ntp_info_.get());
  delete this;
}
