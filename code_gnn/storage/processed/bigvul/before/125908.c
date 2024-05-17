AllDownloadsCompleteObserver::AllDownloadsCompleteObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message,
    DownloadManager* download_manager,
    ListValue* pre_download_ids)
    : provider_(provider->AsWeakPtr()),
      reply_message_(reply_message),
      download_manager_(download_manager) {
  for (ListValue::iterator it = pre_download_ids->begin();
       it != pre_download_ids->end(); ++it) {
    int val = 0;
    if ((*it)->GetAsInteger(&val)) {
      pre_download_ids_.insert(val);
    } else {
      AutomationJSONReply(provider_, reply_message_.release())
          .SendError("Cannot convert ID of prior download to integer.");
      delete this;
      return;
    }
  }
  download_manager_->AddObserver(this);
  DownloadManager::DownloadVector all_items;
  download_manager->GetAllDownloads(&all_items);
  for (DownloadManager::DownloadVector::const_iterator
       it = all_items.begin(); it != all_items.end(); ++it) {
    OnDownloadCreated(download_manager_, *it);
  }
  ReplyIfNecessary();
}
