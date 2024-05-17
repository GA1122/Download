void AutomationProviderDownloadUpdatedObserver::OnDownloadUpdated(
    DownloadItem* download) {
  if (wait_for_open_ && !download->GetAutoOpened())
    return;

  download->RemoveObserver(this);

  if (provider_) {
    scoped_ptr<DictionaryValue> return_value(
        provider_->GetDictionaryFromDownloadItem(download, incognito_));
    AutomationJSONReply(provider_, reply_message_.release()).SendSuccess(
        return_value.get());
  }
  delete this;
}
