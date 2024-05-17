bool Browser::CanCloseWithInProgressDownloads() {
  if (cancel_download_confirmation_state_ != NOT_PROMPTED) {
    if (cancel_download_confirmation_state_ == WAITING_FOR_RESPONSE) {
      return false;
    }
    return true;
  }
  bool normal_downloads_are_present = false;
  bool incognito_downloads_are_present = false;
  CheckDownloadsInProgress(&normal_downloads_are_present,
                      &incognito_downloads_are_present);
  if (!normal_downloads_are_present && !incognito_downloads_are_present)
    return true;

  if (is_attempting_to_close_browser_)
    return true;

  if ((!normal_downloads_are_present && !profile()->IsOffTheRecord()) ||
      (!incognito_downloads_are_present && profile()->IsOffTheRecord()))
    return true;

  int count = 0;
  for (BrowserList::const_iterator iter = BrowserList::begin();
       iter != BrowserList::end(); ++iter) {
    Browser* const browser = *iter;
    if (browser == this
        || browser->is_attempting_to_close_browser_
        || browser->type() != Browser::TYPE_NORMAL)
      continue;

    if (normal_downloads_are_present && !profile()->IsOffTheRecord() &&
        browser->profile()->IsOffTheRecord())
      continue;
    if (incognito_downloads_are_present && profile()->IsOffTheRecord() &&
        !browser->profile()->IsOffTheRecord())
      continue;

    if ((*iter)->profile() == profile() ||
        (*iter)->profile()->GetOriginalProfile() == profile())
      count++;
  }
  if (count > 0)
    return true;

  cancel_download_confirmation_state_ = WAITING_FOR_RESPONSE;
  window_->ConfirmBrowserCloseWithPendingDownloads();

  return false;
}
