void AllDownloadsCompleteObserver::ReplyIfNecessary() {
  if (!pending_downloads_.empty())
    return;

  download_manager_->RemoveObserver(this);
  if (provider_)
    AutomationJSONReply(provider_, reply_message_.release()).SendSuccess(NULL);
  delete this;
}
